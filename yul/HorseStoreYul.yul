
// How to compile Yul code using solc:
// solc --strict-assembly --optimize --optimize-runs 20000 ./yul/HorseStoreYul.yul --bin | grep 60

object "HorseStoreYul" {
    code {
        // Contract Deployment
        datacopy(0, dataoffset("Runtime"), datasize("Runtime"))
        return(0, datasize("Runtime"))
    }
    object "Runtime" {
        code {
            // function dispatcher
            switch selector()
            // updateHorseNumber(uint256)
            case 0xcdfead2e { 
                storeNumber(decodeAsUint256(0))
            }
            // readNumberOfHorses()
            case 0xe026c017 {
                readNumber(0)
            }
            // No fallback function
            default {
                revert(0, 0)
            }
            
            // Store number in storage slot 0
            function storeNumber(num) {
                sstore(0x0, num)
            }

            // Read number from storage slot 0 and return it
            function readNumber(slot) {
                let num := sload(slot)
                mstore(0x0, num)
                return(0, 0x20)
            }

            
            /* Decoding functions */
            
            function selector() -> s {
                // first 4 bytes of calldata
                s := div(calldataload(0), 0x100000000000000000000000000000000000000000000000000000000)
                // this is the same than:
                // s := shr(0x28, calldataload(0))
            }

            // calldata structure: [selector: 4 bytes][param0: 32 bytes][param1: 32 bytes]...
            // 
            // ┌─────────┬─────────────────────────────────────────────┐
            // │ Bytes   │ Content.                                    │
            // ├─────────┼─────────────────────────────────────────────┤
            // │ 0-3     │ 0xcdfead2e (selector)                       │
            // │ 4-35    │ 0x000...07b (123 en uint256, 32 bytes)      │
            // └─────────┴─────────────────────────────────────────────┘
            //
            // offset = 0 (first parameter)
            // positionInCalldata = 4 + (0 * 32) = 4 (starts at byte 4)
            // reads 32 bytes starting from position 4
            //
            // offset = 1 (second parameter, if existed)
            // positionInCalldata = 4 + (1 * 32) = 36 (starts at byte 36)
            
            function decodeAsUint256(offset) -> v {
                let positionInCalldata := add(4, mul(offset, 0x20))
                if lt(calldatasize(), add(positionInCalldata, 0x20)) {
                    revert(0, 0)
                }
                v := calldataload(positionInCalldata)
            }
        }
    }
}