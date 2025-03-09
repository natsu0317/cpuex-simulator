# cpuex Simulator

![alt text](image.png)

## Usage

### Simulation

1. **Prepare Assembly Code**:
   - Place your assembly code in `source/fast_parser/document/assembly.txt`.

2. **Prepare sld file**:
   - Place sld file in `source/fast_parser/document/formatted_sld_data.txt`.

3. **Compile and Run**:
   ```bash
   cd source/fast_parser
   make
   ./fast_parser
   ```

4. **Output**:

- **Binary Code**:
  - The binary representation of the assembly code will be saved in `./document/binary.txt`.

- **Integer Register Transition**:
  - The changes in integer registers during execution will be documented in `./document/transition.md`.

- **Float Register Transition**:
  - The changes in floating-point registers will be recorded in `./document/float-transition.md`.

- **Memory Transition**:
  - The modifications to memory during execution will be logged in `./document/memory_transition.md`.

### SLD File Simulation

1. **Configure Reader**:
    Edit cpuex-v1.6/server/old/reader.py:
    ```bash
    file_name = '../raytracer/sld/temp.sld'
2. **Run the Reader**:
    ```bash
    cd cpuex-v1.6/server/old
    python3 reader.py
3. **Output**:

    Formatted SLD data will be displayed in cpuex-v1.6/server/old/formatted_sld_data.

## Functions

1. **finish**:
 Insert "finish" in your assembly code to terminate execution.
2. **break**:
 Insert "break" to pause execution. Type c to resume.

## Cache Simulation
  ### Direct Mapped Cache
  The direct mapped cache simulation can be complied and run as follows:
  ```bash
  cd cache
  make
  ./direct
  ```

  ### Set Associative Cache
  ```bash
  cd cache
  make
  ./set_associative [ways]
  ```
  The set associative cache simulation allows for flexibility in cache configuration. You can specify the number of ways as a command-line argument. If no argument is provided, the default is 4-way set associative.
  ・[ways]: Optional. Specify the number of ways for the set associative cache. For example, ./set_associative 8 runs the simulation with 8-way associativity. If ommited, the default is 4-way.
