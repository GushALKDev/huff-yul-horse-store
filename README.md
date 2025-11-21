# Horse Store

The main goal of this exercise is to learn how to implement smart contracts in low-level languages like **Huff** and **Yul**, using a **Solidity** contract as a reference model.

We will be using **Huff**, **Solidity**, and **Yul** to do so. Later, this repository will be used to understand the basics of **formal verification**.

## Versions Comparison

This project is divided into two main versions, each with increasing complexity:

*   **Version 1 (V1)**:
    *   **Goal**: Basic storage interaction.
    *   **Features**: Simple functions to read and write the number of horses (`updateHorseNumber`, `readNumberOfHorses`).
    *   **Learning Focus**: Introduction to the syntax of Huff and Yul, and basic stack/storage operations.

*   **Version 2 (V2)**:
    *   **Goal**: Full ERC721 implementation with custom logic.
    *   **Features**:
        *   Minting and NFT management.
        *   `feedHorse`: Updates a timestamp in storage.
        *   `isHappyHorse`: Complex logic checking if a horse was fed within the last 24 hours.
        *   Uses the **Huffmate** library for optimized data structures and utilities.
    *   **Learning Focus**: Advanced memory management, external calls, event emission, and implementing standard interfaces (ERC721) in assembly.

## Getting Started

### Requirements

*   **Git**: You'll know you've done it right if you can run `git --version`.
*   **Foundry / Foundryup**: This will install `forge`, `cast`, and `anvil`.
    *   Test installation: `forge --version`
    *   Update: `foundryup`
*   **Huff Compiler**: You'll know you've done it right if you can run `huffc --version`.
*   **Solidity Compiler**: You'll know you've done it right if you can run `solc --version`.
*   **Halmos** (requires Python): You'll know you've done it right if you can run `halmos --version`.

### Quickstart

1.  **Clone the repo & install dependencies**
    ```bash
    git clone https://github.com/Cyfrin/10-horse-store-s23
    cd 10-horse-store-s23
    make
    ```

2.  **Run tests**
    ```bash
    forge test
    ```

## Use

### Compile Contracts & Get Bytecode

To get the raw bytecode of each contract manually, use the following commands.

> **Note**: To match the course outputs exactly, you may need `solc` version `0.8.20`.

**Huff**
```bash
huffc src/horseStoreV1/HorseStore.huff -b
```

**Yul (Standalone)**
```bash
solc --strict-assembly --optimize --optimize-runs 20000 yul/HorseStoreYul.yul --bin | grep 60
```

**Solidity**
```bash
solc --optimize --optimize-runs 20000 src/horseStoreV1/HorseStore.sol --bin | grep 60
```

**Solidity (Yul Implementation)**
```bash
solc --optimize --optimize-runs 20000 src/horseStoreV1/HorseStoreYul.sol --bin | grep 60
```

## Project Structure

*   **`src/horseStoreV1`**: Contains the initial versions of the Horse Store in Huff, Solidity, and Yul.
*   **`src/horseStoreV2`**: Contains updated versions (likely for later parts of the course).
*   **`yul/`**: Contains standalone Yul implementations.
*   **`breakdowns/`**: Contains opcode breakdowns.

> **Note**: Files in this folder may have the `.c++` extension. This is solely for **syntax highlighting** purposes to make OPCODES easier to read. The code is **not** C++.