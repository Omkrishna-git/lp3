let web3;
let contract;
let currentAccount;
let contractAddress = "YOUR_CONTRACT_ADDRESS"; // Update after deployment

// Contract ABI - Update this after compilation
const contractABI = [
    {
        "inputs": [{"internalType": "string", "name": "_medicalDataHash", "type": "string"}],
        "stateMutability": "nonpayable",
        "type": "constructor"
    },
    {
        "anonymous": false,
        "inputs": [
            {"indexed": true, "internalType": "address", "name": "doctor", "type": "address"},
            {"indexed": false, "internalType": "uint256", "name": "timestamp", "type": "uint256"}
        ],
        "name": "AccessGranted",
        "type": "event"
    },
    {
        "anonymous": false,
        "inputs": [
            {"indexed": true, "internalType": "address", "name": "doctor", "type": "address"},
            {"indexed": false, "internalType": "uint256", "name": "timestamp", "type": "uint256"}
        ],
        "name": "AccessRevoked",
        "type": "event"
    },
    {
        "inputs": [{"internalType": "address", "name": "doctor", "type": "address"}],
        "name": "grantAccess",
        "outputs": [],
        "stateMutability": "nonpayable",
        "type": "function"
    },
    {
        "inputs": [{"internalType": "address", "name": "doctor", "type": "address"}],
        "name": "revokeAccess",
        "outputs": [],
        "stateMutability": "nonpayable",
        "type": "function"
    },
    {
        "inputs": [],
        "name": "viewRecord",
        "outputs": [{"internalType": "string", "name": "", "type": "string"}],
        "stateMutability": "nonpayable",
        "type": "function"
    },
    {
        "inputs": [{"internalType": "string", "name": "newHash", "type": "string"}],
        "name": "updateRecord",
        "outputs": [],
        "stateMutability": "nonpayable",
        "type": "function"
    },
    {
        "inputs": [],
        "name": "getAccessLogs",
        "outputs": [
            {
                "components": [
                    {"internalType": "address", "name": "doctor", "type": "address"},
                    {"internalType": "uint256", "name": "timestamp", "type": "uint256"},
                    {"internalType": "string", "name": "action", "type": "string"}
                ],
                "internalType": "struct MedicalRecord.AccessLog[]",
                "name": "",
                "type": "tuple[]"
            }
        ],
        "stateMutability": "view",
        "type": "function"
    },
    {
        "inputs": [{"internalType": "address", "name": "doctor", "type": "address"}],
        "name": "isAuthorized",
        "outputs": [{"internalType": "bool", "name": "", "type": "bool"}],
        "stateMutability": "view",
        "type": "function"
    },
    {
        "inputs": [],
        "name": "patient",
        "outputs": [{"internalType": "address", "name": "", "type": "address"}],
        "stateMutability": "view",
        "type": "function"
    }
];

window.addEventListener('load', async () => {
    await initWeb3();
});

async function initWeb3() {
    if (typeof window.ethereum !== 'undefined') {
        web3 = new Web3(window.ethereum);
        try {
            await window.ethereum.request({ method: 'eth_requestAccounts' });
            const accounts = await web3.eth.getAccounts();
            currentAccount = accounts[0];
            
            contract = new web3.eth.Contract(contractABI, contractAddress);
            
            document.getElementById('currentAccount').textContent = currentAccount;
            
            await checkAccountType();
            
            showMessage('Connected successfully!', 'success');
        } catch (error) {
            showMessage('User denied account access', 'error');
        }
    } else {
        showMessage('Please install MetaMask!', 'error');
    }
}

async function checkAccountType() {
    try {
        const patientAddress = await contract.methods.patient().call();
        
        if (currentAccount.toLowerCase() === patientAddress.toLowerCase()) {
            document.getElementById('accountType').textContent = 'Patient';
            document.getElementById('patientDashboard').classList.remove('hidden');
            document.getElementById('doctorDashboard').classList.add('hidden');
        } else {
            document.getElementById('accountType').textContent = 'Doctor';
            document.getElementById('doctorDashboard').classList.remove('hidden');
            document.getElementById('patientDashboard').classList.add('hidden');
        }
    } catch (error) {
        console.error('Error checking account type:', error);
    }
}

async function grantAccess() {
    const doctorAddress = document.getElementById('doctorAddress').value;
    
    if (!web3.utils.isAddress(doctorAddress)) {
        showMessage('Invalid Ethereum address', 'error');
        return;
    }
    
    try {
        await contract.methods.grantAccess(doctorAddress).send({ from: currentAccount });
        showMessage(`Access granted to ${doctorAddress}`, 'success');
        document.getElementById('doctorAddress').value = '';
    } catch (error) {
        showMessage('Error granting access: ' + error.message, 'error');
    }
}

async function revokeAccess() {
    const doctorAddress = document.getElementById('revokeDoctorAddress').value;
    
    if (!web3.utils.isAddress(doctorAddress)) {
        showMessage('Invalid Ethereum address', 'error');
        return;
    }
    
    try {
        await contract.methods.revokeAccess(doctorAddress).send({ from: currentAccount });
        showMessage(`Access revoked from ${doctorAddress}`, 'success');
        document.getElementById('revokeDoctorAddress').value = '';
    } catch (error) {
        showMessage('Error revoking access: ' + error.message, 'error');
    }
}

async function updateRecord() {
    const medicalHash = document.getElementById('medicalHash').value;
    
    if (!medicalHash) {
        showMessage('Please enter a medical record hash', 'error');
        return;
    }
    
    try {
        await contract.methods.updateRecord(medicalHash).send({ from: currentAccount });
        showMessage('Medical record updated successfully', 'success');
        document.getElementById('medicalHash').value = '';
    } catch (error) {
        showMessage('Error updating record: ' + error.message, 'error');
    }
}

async function viewRecord() {
    try {
        const record = await contract.methods.viewRecord().send({ from: currentAccount });
        
        // Get the actual hash from transaction receipt
        const receipt = await web3.eth.getTransactionReceipt(record.transactionHash);
        
        // Alternative: call the method to get return value
        const recordHash = await contract.methods.viewRecord().call({ from: currentAccount });
        
        const displayElement = document.getElementById('recordDisplay') || document.getElementById('doctorRecordDisplay');
        displayElement.innerHTML = `
            <h4>Medical Record Hash:</h4>
            <p class="hash">${recordHash}</p>
            <p class="info">This hash can be used to retrieve the actual medical record from IPFS</p>
        `;
        showMessage('Record retrieved successfully', 'success');
    } catch (error) {
        showMessage('Error viewing record: ' + error.message, 'error');
    }
}

async function getAccessLogs() {
    try {
        const logs = await contract.methods.getAccessLogs().call();
        
        let logsHTML = '<table><tr><th>Doctor Address</th><th>Timestamp</th><th>Action</th></tr>';
        
        for (let log of logs) {
            const date = new Date(log.timestamp * 1000);
            logsHTML += `
                <tr>
                    <td>${log.doctor}</td>
                    <td>${date.toLocaleString()}</td>
                    <td>${log.action}</td>
                </tr>
            `;
        }
        
        logsHTML += '</table>';
        document.getElementById('accessLogs').innerHTML = logsHTML;
        showMessage('Access logs loaded', 'success');
    } catch (error) {
        showMessage('Error loading logs: ' + error.message, 'error');
    }
}

async function checkAuthorization() {
    try {
        const isAuth = await contract.methods.isAuthorized(currentAccount).call();
        const statusElement = document.getElementById('authStatus');
        
        if (isAuth) {
            statusElement.textContent = '✅ You are authorized to view patient records';
            statusElement.className = 'status-authorized';
        } else {
            statusElement.textContent = '❌ You are NOT authorized to view patient records';
            statusElement.className = 'status-unauthorized';
        }
    } catch (error) {
        showMessage('Error checking authorization: ' + error.message, 'error');
    }
}

function showMessage(message, type) {
    const messagesDiv = document.getElementById('statusMessages');
    const messageElement = document.createElement('div');
    messageElement.className = `message ${type}`;
    messageElement.textContent = message;
    messagesDiv.appendChild(messageElement);
    
    setTimeout(() => {
        messageElement.remove();
    }, 5000);
}
