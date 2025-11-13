// SPDX-License-Identifier: GLP-3.0-only
pragma solidity ^0.8.28;

import {Base_TestV1, IHorseStore} from "./Base_TestV1.t.sol";
import {HorseStore} from "../../src/horseStoreV1/HorseStoreYul.sol";

contract HorseStoreYul is Base_TestV1 {
    function setUp() public override {
        horseStore = IHorseStore(address(new HorseStore()));
    }
}