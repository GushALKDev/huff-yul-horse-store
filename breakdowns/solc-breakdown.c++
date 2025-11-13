// Used c++ extension because the syntax highlighter in github is pretty nice for opcodes

// 6080604052348015600e575f80fd5b5060a58061001b5f395ff3fe6080604052348015600e575f80fd5b50600436106030575f3560e01c8063cdfead2e146034578063e026c017146045575b5f80fd5b6043603f3660046059565b5f55565b005b5f5460405190815260200160405180910390f35b5f602082840312156068575f80fd5b503591905056fe

// 3 sections:
// 1. Contract Creation Code
// 2. Runtime Code
// 3. Contract Metadata

// 1. CONTRACT CREATION CODE
// We break down all the opcodes of our solidity contract
// You can find the final opcodes in `solidity-breakdown.md`

// Free Memory Pointer
// 0x40 is always a reserved slot for the free memory pointer
// Slots 0x00 and 0x20 are reserved for hashing stuff and other Solidity internal stuff
// It indicates where the free memory starts. It starts at 0x80
PUSH1 0x80          // [80]
PUSH1 0x40          // [40, 80]
MSTORE              // []                                   // Memory [0x40 = 0x80]

// If someone sends value with this call revert!
// Otherwise, continue the execution (deploy the contract)
CALLVALUE           // [msg.value]
DUP1                // [msg.value, msg.value]
ISZERO              // [msg.value == 0, msg.value]
PUSH1 0x0e          // [0x0E (JUMPDEST), msg.value == 0, msg.value]
JUMPI               // [msg.value]
// We only reach this part if msg.value has value, and will revert the TX
PUSH0               // [0x00, msg.value]                    // Error size 0
DUP1                // [0x00, 0x00, msg.value]              // Error position offset 0
REVERT              // [msg.value]

// Jump dest if msg.value == 0
JUMPDEST            // [msg.value]
POP                 // []
PUSH1 0x0129        // [0x0129]
DUP1                // [0x0129, 0x0129]
PUSH2 0x001b        // [0x001B, 0x0129, 0x0129]
PUSH0               // [0x00, 0x001B, 0x0129, 0x0129]       // offset in memory (0), offset in code (0x001B), size (0x0129)
CODECOPY            // [0x0129]                             // Memory: [Runtime code copied here]
PUSH0               // [0x00, 0x0129]
RETURN              // []                                   // Return runtime code in memory [0, 0x0129] [offset, size]
INVALID             // []
// 1. END CONTRACT CREATION CODE

// 2. RUNTIME CODE (Entry point of all calls to the contract)
// Free Memory Pointer
PUSH1 0x80          // [80]
PUSH1 0x40          // [40, 80]
MSTORE              // []                                   // Memory [0x40 = 0x80]

// Checking for msg.value, revert if any value is sent, as no functions are payable
CALLVALUE           // [msg.value]
DUP1                // [msg.value, msg.value]
ISZERO              // [msg.value == 0, msg.value]
PUSH1 0x0e          // [0x0E (JUMPDEST), msg.value == 0, msg.value]
JUMPI               // [msg.value]
// Jump to continue if msg.value == 0

// We only reach this part if msg.value has value (msg.value != 0), and will revert the TX
PUSH0               // [0x00, msg.value]
PUSH0               // [0x00, 0x00, msg.value]
REVERT              // [msg.value]

// Jump dest if msg.value == 0
JUMPDEST            // [msg.value]
POP                 // []
PUSH1 0x04          // [0x04]
CALLDATASIZE        // [calldatasize(bytes), 0x04]
LT                  // [calldatasize(bytes) < 0x04]             // If calldata size is less than 4 bytes, we can't have a valid function selector
PUSH1 0x30          // [0x30, (1 or 0)]                         // Jump to revert if true
JUMPI               // []                                       // Continue if false
// Jump to next JUMPDEST (0x30) if calldata size >= 4 bytes

PUSH0               // [0x00]
CALLDATALOAD        // [calldataload]                           // Call data load with offset 0 (first 32 bytes of calldata)
PUSH1 0xe0          // [0xe0, calldataload]
SHR                 // [function_selector]                      // Whole calldata (32 x 8 == 256) Shift Right (0xe0 == 224) == 32 bits == 4 bytes (function selector)

// function selector for updateHorseNumber(uint256)
DUP1                // [function_selector, function_selector]
PUSH4 0xcdfead2e    // [0xcdfead2e, function_selector, function_selector]
EQ                  // [function_selector == 0xcdfead2e, function_selector]
PUSH1 0x34          // [0x34, (1 or 0), function_selector]
JUMPI               // [function_selector]
// JUMP to 0x34 if function selector matches updateHorseNumber(uint256)

// function selector for getHorseNumber(uint256)
DUP1                // [function_selector, function_selector]
PUSH4 0xe026c017    // [0xe026c017, function_selector, function_selector]
EQ                  // [function_selector == 0xe026c017, function_selector]
PUSH1 0x45          // [0x45, (1 or 0), function_selector]
JUMPI               // [function_selector]
// JUMP to 0x45 if function selector matches getHorseNumber()


// JUMPDEST to revert if:
// - calldata size is less than 4 bytes
// - function selector doesn't match any function
JUMPDEST            // [function_selector]
PUSH0               // [0x00, function_selector]
PUSH0               // [0x00, 0x00, function_selector]
REVERT              // [function_selector]
// END OF FUNCTION SELECTOR DISPATCHING


// JUMPDEST 0X34 (updateHorseNumber(uint256))
JUMPDEST            // [function_selector]
PUSH1 0x43          // [0x43, function_selector]
PUSH1 0x3f          // [0x3f, 0x43, function_selector]
CALLDATASIZE        // [calldatasize, 0x3f, 0x43, function_selector]
PUSH1 0x04          // [0x04, calldatasize, 0x3f, 0x43, function_selector]
PUSH1 0x59          // [0x59, 0x04, calldatasize, 0x3f, 0x43, function_selector]
JUMP                // [0x04, calldatasize, 0x3f, 0x43, function_selector]
// JUMP to 0x59


// JUMPDEST 0x3f (updateHorseNumber(uint256))
// Store the new horse number in storage slot 0
JUMPDEST            // [calldataload(without_function_selector), 0x4a, function_selector]
PUSH0               // [0x00, calldataload(without_function_selector), 0x4a, function_selector]
SSTORE              // [0x4a, function_selector]
JUMP                // [function_selector]
// JUMP to 0x4a

// JUMPDEST 0x4a (updateHorseNumber(uint256))
JUMPDEST            // [function_selector]
STOP                // [function_selector]
// END OF EXECUTION


// JUMPDEST 0x45 (readNumberOfHorses())
JUMPDEST            // [function_selector]
PUSH0               // [0x00, function_selector]
SLOAD               // [storage_value_at_slot_0, function_selector]
PUSH1 0x40          // [0x40, storage_value_at_slot_0, function_selector]
MLOAD               // [free_memory_pointer, storage_value_at_slot_0, function_selector]                // Load free memory pointer 
SWAP1               // [storage_value_at_slot_0, free_memory_pointer, function_selector]
DUP2                // [free_memory_pointer, storage_value_at_slot_0, free_memory_pointer, function_selector]
MSTORE              // [free_memory_pointer, function_selector]                                         // Store storage value at free memory pointer]
PUSH1 0x20          // [0x20, free_memory_pointer, function_selector]                                   // Push memory size (32 bytes)
ADD                 // [new_free_memory_pointer, function_selector]                                     // Calculate new free memory pointer
PUSH1 0x40          // [0x40, new_free_memory_pointer, function_selector]
MLOAD               // [old_free_memory_pointer, new_free_memory_pointer, function_selector]            // Load old free memory pointer
DUP1                // [old_free_memory_pointer, old_free_memory_pointer, new_free_memory_pointer, function_selector]
SWAP2               // [new_free_memory_pointer, old_free_memory_pointer, old_free_memory_pointer, function_selector]
SUB                 // [last_saved_memory_size, old_free_memory_pointer, function_selector]
SWAP1               // [old_free_memory_pointer, last_saved_memory_size, function_selector]              // [0x80, 0x20, function_selector]
RETURN              // [function_selector]                                                               // Return [offset, size]


// JUMPDEST 0x59 (updateHorseNumber(uint256))
// We check to see there is enough calldata for the function (not including the function selector)
JUMPDEST            // [0x04, calldatasize, 0x3f, 0x43, function_selector]
PUSH0               // [0x00, 0x04, calldatasize, 0x3f, 0x43, function_selector]
PUSH1 0x20          // [0x20, 0x00, 0x04, calldatasize, 0x3f, 0x43, function_selector]
DUP3                // [0x04, 0x20, 0x00, 0x04, calldatasize, 0x3f, 0x43, function_selector]
DUP5                // [calldatasize, 0x04, 0x20, 0x00, 0x04, calldatasize, 0x3f, 0x43, function_selector]
SUB                 // [calldatasize - 0x04, 0x20, 0x00, 0x04, calldatasize, 0x3f, 0x43, function_selector]
// Is there more data than just the function selector?
// If there is more there will be at least 32 bytes (0x20) not less.
SLT                 // [(calldatasize - 0x04) < 0x20, 0x00, 0x04, calldatasize, 0x3f, 0x4a, function_selector]
ISZERO              // [more_calldata_than_selector?, 0x00, 0x04, calldatasize, 0x3f, 0x4a, function_selector]
PUSH1 0x68          // [0x68, more_calldata_than_selector?, 0x00, 0x04, calldatasize, 0x3f, 0x4a, function_selector]
JUMPI               // [0x00, 0x04, calldatasize, 0x3f, 0x4a, function_selector]
// JUMP to 0x68 if there is enough calldata


// Not enough calldata, revert
PUSH0               // [0x00, 0x00, 0x04, calldatasize, 0x3f, 0x4a, function_selector]
DUP1                // [0x00, 0x00, 0x00, 0x04, calldatasize, 0x3f, 0x4a, function_selector]
REVERT              // [0x00, 0x04, calldatasize, 0x3f, 0x4a, function_selector]                // Revert with empty error message, 0x00 offset and 0x00 size
// END of not enough calldata revert]


// JUMPDEST 0x68 (updateHorseNumber(uint256))
// This gets the calldata, without the function selector
JUMPDEST            // [0x00, 0x04, calldatasize, 0x3f, 0x4a, function_selector]
POP                 // [0x04, calldatasize, 0x3f, 0x4a, function_selector]
CALLDATALOAD        // [calldataload(without_function_selector), calldatasize, 0x3f, 0x4a, function_selector]
SWAP2               // [0x3f, calldatasize, calldataload(without_function_selector), 0x4a, function_selector]
SWAP1               // [calldatasize, 0x3f, calldataload(without_function_selector), 0x4a, function_selector]
POP                 // [0x3f, calldataload(without_function_selector), 0x4a, function_selector]
JUMP                // [calldataload(without_function_selector), 0x4a, function_selector]
// JUMP to 0x3f

INVALID

