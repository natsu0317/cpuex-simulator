# cpuex Simulator

## Usage

### Pipeline Simulation

1. **Prepare Assembly Code**:
   - Place your assembly code in `source/pipeline/document/assembly.txt`.

2. **Prepare sld file**:
   - Place sld file in `source/pipeline/document/formatted_sld_data.txt`.

3. **Compile and Run**:
   ```bash
   cd source/pipeline
   make
   ./pipeline
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

- **Pipeline Simulation**:
  - The simulation of the pipeline, including stages and stalls, will be detailed in `./document/pipeline.txt`.

- **Instruction Statistics**:
  - Statistics for each instruction, such as execution count and cycles, will be available in `./document/instruction_statics.txt`.



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

### PPM Conversion and Error Checking
1. **Convert to PPM**:
    Navigate to the source/to_ppm directory and run the following commands to convert data into a PPM file:
    ```bash
    cd source/to_ppm
    make
    ./ppm
    ```
    The output will be saved as output.ppm.

2. **Integer Sequence Preparation**:

    The program also generates an int_file with integers listed on a single line. This file is used for error checking against a correct sequence.

3. **Prepare Correct Sequence**:

    Place the correct sequence of integers in correct.txt.

4. **Check for Mistakes**:

    Run the following command to compare the sequence in int_file with correct.txt:
    ```bash
    ./search_mistake
    ```

    The terminal will display the positions and values of any discrepancies found.

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
