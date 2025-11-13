// SPDX-License-Identifier: GLP-3.0-only
pragma solidity ^0.8.28;

import {IHorseStore} from "../../src/horseStoreV1/IHorseStore.sol";
import "forge-std/Test.sol";

abstract contract Base_TestV1 is Test {
    IHorseStore public horseStore;

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