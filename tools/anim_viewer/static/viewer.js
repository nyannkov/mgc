const canvas = document.getElementById("viewer");
const ctx = canvas.getContext("2d");

const selector = document.getElementById("animSelector");
const yamlNameElem = document.getElementById("yamlName");
const animLabelElem = document.getElementById("animLabel");
const descLabelElem = document.getElementById("descLabel");
const animDescElem = document.getElementById("animDesc");
const frameIndexElem = document.getElementById("frameIndex");
const frameCountElem = document.getElementById("frameCount");
const frameNameElem = document.getElementById("frameName");
const frameTimeElem = document.getElementById("frameTime");

const loopToggle = document.getElementById("loopToggle");
const playBtn = document.getElementById("playBtn");
const prevBtn = document.getElementById("prevBtn");
const nextBtn = document.getElementById("nextBtn");
const resetBtn = document.getElementById("resetBtn");
const speedSlider = document.getElementById("speedSlider");
const speedLabel = document.getElementById("speedLabel");

let scale = 2;
const minScale = 0.5;
const maxScale = 10;
const savedScale = parseFloat(localStorage.getItem("animScale"));
if (!isNaN(savedScale)) {
    scale = Math.max(minScale, Math.min(maxScale, savedScale));
}


let yamlData;
let yamlFileName;
let currentAnimIndex = 0;
let frameImagesCache = {};


const PlayMode = Object.freeze({
    PLAY: "Play",
    STOP: "Stop"
});

let playMode = PlayMode.PLAY;
nextBtn.disabled = true;
prevBtn.disabled = true;
resetBtn.disabled = true;

let finished = false;
let paused = false;
let frameIndex = 0;
let speed = 1.0;


async function loadYamlAsJson() {
    const resp = await fetch("/api/anim");
    return await resp.json();
}

async function loadImage(src) {
    return new Promise((resolve) => {
        const img = new Image();
        img.onload = () => resolve(img);
        img.src = src;
    });
}

async function loadAnimFrames(anim) {
    if (frameImagesCache[anim.name]) return frameImagesCache[anim.name];

    const frames = await Promise.all(
        anim.frames.map(f => loadImage("/res/" + f.file))
    );
    frameImagesCache[anim.name] = frames;
    return frames;
}

function drawFrame(img) {
    ctx.clearRect(0, 0, canvas.width, canvas.height);

    const w = img.width * scale;
    const h = img.height * scale;
    const x = (canvas.width - w) / 2;
    const y = (canvas.height - h) / 2;

    ctx.imageSmoothingEnabled = false;
    ctx.drawImage(img, x, y, w, h);
}

async function playAnimation(animIndex) {
    const anim = yamlData.animations[animIndex];
    const frames = await loadAnimFrames(anim);

    yamlNameElem.textContent = yamlFileName;
    animLabelElem.textContent = "Animation: " + anim.name;
    animDescElem.textContent = anim.description || "---";

    frameCountElem.textContent = frames.length;
    let lastTime = performance.now();

    async function loop(time) {
        const loopEnabled = loopToggle.checked;

        if (playMode === PlayMode.PLAY) {
            const frame = anim.frames[frameIndex];
            const absSpeed = Math.abs(speed);
            const duration = frame.time_ms / (absSpeed > 0.001 ? absSpeed : 1e9);

            drawFrame(frames[frameIndex]);
            frameIndexElem.textContent = frameIndex;
            frameNameElem.textContent = frame.name || "---";
            frameTimeElem.textContent = frame.time_ms;

            if (!paused && speed !== 0 && (time - lastTime > duration)) {
                lastTime = time;

                if (speed > 0) {
                    if (frameIndex + 1 < frames.length) {
                        frameIndex++;
                    } else {
                        if (loopEnabled) {
                            frameIndex = 0;
                        } else {
                            frameIndex = frames.length - 1;
                            finished = true;
                        }
                    }
                } else {
                    if (frameIndex - 1 >= 0) {
                        frameIndex--;
                    } else {
                        if (loopEnabled) {
                            frameIndex = frames.length - 1;
                        } else {
                            frameIndex = 0;
                            finished = true;
                        }
                    }
                }
            }
        } else if (playMode === PlayMode.STOP) {
            const frame = anim.frames[frameIndex];
            drawFrame(frames[frameIndex]);
            frameIndexElem.textContent = frameIndex;
            frameNameElem.textContent = frame.name || "---";
            frameTimeElem.textContent = frame.time_ms;
        }

        if (animIndex !== currentAnimIndex) return;
        requestAnimationFrame(loop);
    }

    requestAnimationFrame(loop);
}

selector.addEventListener("change", (e) => {
    currentAnimIndex = parseInt(e.target.value, 10);
    playAnimation(currentAnimIndex);
});

playBtn.addEventListener("click", () => {

    paused = !paused;

    if ( paused ) {
        playMode = PlayMode.STOP;
        playBtn.textContent = PlayMode.PLAY;
        prevBtn.disabled = false;
        nextBtn.disabled = false;
		resetBtn.disabled = false;
    } else {
        playMode = PlayMode.PLAY;
        playBtn.textContent = PlayMode.STOP;
        prevBtn.disabled = true;
        nextBtn.disabled = true;
		resetBtn.disabled = true;
    }
});

prevBtn.addEventListener("click", () => {
    if (playMode === PlayMode.STOP) {
        const anim = yamlData.animations[currentAnimIndex];
        const loopEnabled = loopToggle.checked;
        if (frameIndex > 0) {
            frameIndex--;
        } else if (loopEnabled) {
            frameIndex = anim.frames.length - 1;
        }
    }
});

nextBtn.addEventListener("click", () => {
    if (playMode === PlayMode.STOP) {
        const anim = yamlData.animations[currentAnimIndex];
        const loopEnabled = loopToggle.checked;
        if (frameIndex < anim.frames.length - 1) {
            frameIndex++;
        } else if (loopEnabled) {
            frameIndex = 0;
        }
    }
});


resetBtn.addEventListener("click", () => {
    if (playMode === PlayMode.STOP) {
        if ( speed >= 0 ) {
            frameIndex = 0;
        } else {
            const anim = yamlData.animations[currentAnimIndex];
            frameIndex = anim.frames.length - 1;
        }
    }
    frameIndexElem.textContent = frameIndex;
});


canvas.addEventListener("wheel", (e) => {
    e.preventDefault();
    scale *= (e.deltaY < 0) ? 1.2 : 0.8333;
    scale = Math.max(minScale, Math.min(maxScale, scale));
	localStorage.setItem("animScale", scale);
});

speedSlider.addEventListener("input", (e) => {
    speed = parseFloat(e.target.value) || 0;
    speedLabel.textContent = speed.toFixed(1) + "x";
});

async function main() {

    info = await loadYamlAsJson();

	yamlData = info.data;
    yamlFileName = info.yaml_name;

    yamlData.animations.forEach((anim, idx) => {
        const opt = document.createElement("option");
        opt.value = idx;
        opt.textContent = anim.name;
        selector.appendChild(opt);
    });

    playBtn.disabled = false;
    playAnimation(currentAnimIndex);
}

main();

