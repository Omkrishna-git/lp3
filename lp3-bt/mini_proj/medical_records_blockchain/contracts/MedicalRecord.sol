// SPDX-License-Identifier: MIT
pragma solidity ^0.8.0;

contract MedicalRecord {
    address public patient;
    mapping(address => bool) public authorizedDoctors;
    string private medicalDataHash; // Encrypted hash of the medical record
    
    struct AccessLog {
        address doctor;
        uint256 timestamp;
        string action;
    }
    
    AccessLog[] public accessLogs;

    event AccessGranted(address indexed doctor, uint256 timestamp);
    event AccessRevoked(address indexed doctor, uint256 timestamp);
    event RecordAccessed(address indexed doctor, uint256 timestamp);
    event RecordUpdated(string newHash, uint256 timestamp);

    constructor(string memory _medicalDataHash) {
        patient = msg.sender;
        medicalDataHash = _medicalDataHash;
    }

    modifier onlyPatient() {
        require(msg.sender == patient, "Not authorized: Only patient can perform this action");
        _;
    }
    
    modifier onlyAuthorized() {
        require(authorizedDoctors[msg.sender] || msg.sender == patient, "Access denied: Not authorized to view records");
        _;
    }

    function grantAccess(address doctor) public onlyPatient {
        require(doctor != address(0), "Invalid doctor address");
        require(!authorizedDoctors[doctor], "Doctor already has access");
        
        authorizedDoctors[doctor] = true;
        accessLogs.push(AccessLog(doctor, block.timestamp, "Access Granted"));
        emit AccessGranted(doctor, block.timestamp);
    }

    function revokeAccess(address doctor) public onlyPatient {
        require(authorizedDoctors[doctor], "Doctor does not have access");
        
        authorizedDoctors[doctor] = false;
        accessLogs.push(AccessLog(doctor, block.timestamp, "Access Revoked"));
        emit AccessRevoked(doctor, block.timestamp);
    }

    function viewRecord() public onlyAuthorized returns (string memory) {
        accessLogs.push(AccessLog(msg.sender, block.timestamp, "Record Viewed"));
        emit RecordAccessed(msg.sender, block.timestamp);
        return medicalDataHash;
    }
    
    function updateRecord(string memory newHash) public onlyPatient {
        medicalDataHash = newHash;
        accessLogs.push(AccessLog(msg.sender, block.timestamp, "Record Updated"));
        emit RecordUpdated(newHash, block.timestamp);
    }
    
    function getAccessLogs() public view returns (AccessLog[] memory) {
        return accessLogs;
    }
    
    function isAuthorized(address doctor) public view returns (bool) {
        return authorizedDoctors[doctor];
    }
    
    function getPatient() public view returns (address) {
        return patient;
    }
}
