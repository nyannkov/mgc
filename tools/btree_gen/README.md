# btree_gen.py

This script is a utility for generating behavior trees in the mgc library from human-readable YAML files.
It converts a YAML tree definition into static C structure constants (mgc_btree_t) and outputs .c and .h source files suitable for embedded systems.


## YAML Format
The input is a YAML file defining a behavior tree with labeled nodes.
The tree must begin with a special root label inside a btree: block.
Subsequent labels represent internal or leaf nodes and can use any valid name.

Each node definition must include a type and can optionally include a description, children, or id, depending on the node type.

### Supported Node Types:
 - selector: Tries each child in order and returns when one succeeds
 - sequencer: Runs each child in order, halting on failure
 - leaf: Leaf node representing a condition or action


## Example

```yaml
btree:
  root:
    type: selector
    description: "This is the root selector node that decides what action to take based on conditions."
    children:
      - sleep_if_too_long
      - dance_if_long_idle
      - idle_anim

  sleep_if_too_long:
    type: sequencer
    description: "Checks if the character has been idle too long, and if so, performs the sleep action."
    children:
      - is_idle_for_60_sec
      - do_sleep

  dance_if_long_idle:
    type: sequencer
    description: "Checks if the character has been idle for a long time and performs the dance action."
    children:
      - is_idle_for_30_sec
      - do_dance

  idle_anim:
    type: leaf
    description: "Performs idle breathing animation when no actions are being performed."
    id: idle_breathing

  is_idle_for_30_sec:
    type: leaf
    description: "Condition that checks if the character has been idle for more than 30 seconds."
    id: cond/timer/over_30s

  is_idle_for_60_sec:
    type: leaf
    description: "Condition that checks if the character has been idle for more than 60 seconds."
    id: cond/timer/over_60s

  do_dance:
    type: leaf
    description: "Performs the dance animation when the character is idle for too long."
    id: action/dance

  do_sleep:
    type: leaf
    description: "Performs the sleep animation when the character has been idle for too long."
    id: action/sleep

```

## How to use

```bash
python3 btree_gen.py ./btree_yaml/idle_behavior.yaml --dir ./generated
```

### This will generate:

 - idle_behavior.h: Header with extern declarations, macro definitions, and node table
 - idle_behavior.c: Source file defining the node structure and tree root (mgc_btree_t)
 - The generated code is compatible with mgc/behavior/btree.h.


### Notes
 - The root node must be labeled root under the btree: key.
 - description fields are inserted as comments in the generated .c file.
 - Leaf nodes must have an id, which will be emitted as a string for runtime identification.
