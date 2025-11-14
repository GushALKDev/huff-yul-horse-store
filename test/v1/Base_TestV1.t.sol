// SPDX-License-Identifier: GLP-3.0-only
pragma solidity ^0.8.28;

import {IHorseStore} from "../../src/horseStoreV1/IHorseStore.sol";
import "forge-std/Test.sol";

abstract contract Base_TestV1 is Test {
    IHorseStore public horseStore;

    // It's posible to get the bytecode of a Yul contract and use it in the test deployment
    // bytes yulCode = '5f803560e01c8063cdfead2e1460245763e026c01714601b5780fd5b80602091548152f35b50602436106033576004359055005b80fd';

    function setUp() public virtual {}

    function testReadValue() public view {
        uint256 initialValue = horseStore.readNumberOfHorses();
        assertEq(initialValue, 0);
    }

    function testWriteValue(uint256 numberOfHorses) public {
        horseStore.updateHorseNumber(numberOfHorses);
        uint256 updatedValue = horseStore.readNumberOfHorses();
        assertEq(updatedValue, numberOfHorses);
    }
}