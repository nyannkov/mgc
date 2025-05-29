# talkscript_gen.py

This script is a utility for handling conversation scripts in mgc using YAML files.  
It converts human-readable YAML definitions into static C structure constants (`mgc_talkscript_t`) and outputs them as C source code.

## Talk script YAML file

The input is a YAML file containing a structured definition of the conversation flow.  
Each script consists of nodes such as `message`, `choice`, and `decision`, and may include metadata like tags, values, and jump labels.

A basic example is shown below:

```yaml
talkscript:
  subject: "ShopDialogue"
  description: "A simple example of a shopping conversation."

  talknodes:
    start:
      - message: "Welcome! Would you like to buy something?"
      - choice:
          items:
            - { item: "Yes, please.", next: buy }
            - { item: "No, thank you.", next: end }

    buy:
      - { message: "We have apples and bananas." }
      - choice:
          tag: ITEM_SELECT
          items:
            - { item: "Apple", value: 100, tag: APPLE }
            - { item: "Banana", value: 80, tag: BANANA }
            - { item: "Never mind.", next: end }

      - { message: "That will be %u yen. Would you like to proceed?", format: true, tag: PRICE }
      - choice:
          items:
            - { item: "Yes", next: confirm }
            - { item: "No", next: end }

    confirm:
      - message: "Thank you for your purchase!"
      - decision:
          tag: IS_LAST_CUSTOMER
          branches:
            true: end
            false: start

    end:
      - message: "Goodbye!"
        end: true
```

Each label defines a sequence of nodes, and branching can be controlled via choice or decision.


## Examples of use

```bash
python3 talkscript_gen.py ./talkscripts/shop.yaml --dir ./generates
```

This example reads the YAML file shop.yaml and generates the corresponding .h and .c files in ./generates.
These files contain static C structures and macros representing the dialogue script.


