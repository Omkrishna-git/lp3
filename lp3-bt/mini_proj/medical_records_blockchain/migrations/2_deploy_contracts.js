const MedicalRecord = artifacts.require("MedicalRecord");

module.exports = function(deployer) {
  // Deploy with a sample encrypted hash
  const initialHash = "QmSampleEncryptedMedicalDataHash123456789";
  deployer.deploy(MedicalRecord, initialHash);
};
