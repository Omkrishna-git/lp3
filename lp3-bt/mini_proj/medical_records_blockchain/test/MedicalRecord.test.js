const MedicalRecord = artifacts.require("MedicalRecord");

contract("MedicalRecord", (accounts) => {
    let medicalRecordInstance;
    const patient = accounts[0];
    const doctor1 = accounts[1];
    const doctor2 = accounts[2];
    const initialHash = "QmTestHash123";

    beforeEach(async () => {
        medicalRecordInstance = await MedicalRecord.new(initialHash, { from: patient });
    });

    it("should set the patient correctly", async () => {
        const patientAddress = await medicalRecordInstance.patient();
        assert.equal(patientAddress, patient, "Patient address should match");
    });

    it("should allow patient to grant access to a doctor", async () => {
        await medicalRecordInstance.grantAccess(doctor1, { from: patient });
        const isAuthorized = await medicalRecordInstance.isAuthorized(doctor1);
        assert.equal(isAuthorized, true, "Doctor should be authorized");
    });

    it("should allow patient to revoke access from a doctor", async () => {
        await medicalRecordInstance.grantAccess(doctor1, { from: patient });
        await medicalRecordInstance.revokeAccess(doctor1, { from: patient });
        const isAuthorized = await medicalRecordInstance.isAuthorized(doctor1);
        assert.equal(isAuthorized, false, "Doctor should not be authorized");
    });

    it("should allow authorized doctor to view record", async () => {
        await medicalRecordInstance.grantAccess(doctor1, { from: patient });
        const record = await medicalRecordInstance.viewRecord.call({ from: doctor1 });
        assert.equal(record, initialHash, "Record hash should match");
    });

    it("should prevent unauthorized doctor from viewing record", async () => {
        try {
            await medicalRecordInstance.viewRecord({ from: doctor2 });
            assert.fail("Should have thrown an error");
        } catch (error) {
            assert(error.message.includes("Access denied"), "Error message should contain 'Access denied'");
        }
    });

    it("should allow patient to update record", async () => {
        const newHash = "QmNewTestHash456";
        await medicalRecordInstance.updateRecord(newHash, { from: patient });
        
        const record = await medicalRecordInstance.viewRecord.call({ from: patient });
        assert.equal(record, newHash, "Record should be updated");
    });

    it("should log access events", async () => {
        await medicalRecordInstance.grantAccess(doctor1, { from: patient });
        await medicalRecordInstance.viewRecord({ from: doctor1 });
        
        const logs = await medicalRecordInstance.getAccessLogs();
        assert(logs.length > 0, "Should have access logs");
    });
});
