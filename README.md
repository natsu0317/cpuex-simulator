# cpuex Simulator

## Usage

### Pipeline Simulation

1. **Prepare Assembly Code**:
   - Place your assembly code in `source/pipeline/document/assembly.txt`.

2. **Compile and Run**:
   ```bash
   cd source/pipeline
   make
   ./pipeline

3. **Output**:

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



###SLD File Simulation

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
 Insert "break" to pause execution. Type continue to resume.

## Cache Simulation

1. **Compile and Run**:
    ```bash
    cd cahe
    make
    ./direct
