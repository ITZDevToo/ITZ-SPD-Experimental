

// Copyright (c) 2014-2015 The Dash developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#ifndef MASTERNODE_PAYMENTS_H
#define MASTERNODE_PAYMENTS_H

#include "sync.h"
#include "net.h"
#include "key.h"
#include "util.h"
#include "base58.h"
#include "main.h"
#include "masternode.h"

using namespace std;

class CMasternodePayments;
class CMasternodePaymentWinner;

extern CMasternodePayments masternodePayments;
extern map<uint256, CMasternodePaymentWinner> mapSeenMasternodeVotes;

void ProcessMessageMasternodePayments(CNode* pfrom, std::string& strCommand, CDataStream& vRecv);


// for storing the winning payments
class CMasternodePaymentWinner
{
public:
    int nBlockHeight;
    CTxIn vin;
    CScript payee;
    std::vector<unsigned char> vchSig;
    uint64_t score;

    CMasternodePaymentWinner() {
        nBlockHeight = 0;
        score = 0;
        vin = CTxIn();
        payee = CScript();
    }

    uint256 GetHash(){
        uint256 n2 = Hash(BEGIN(nBlockHeight), END(nBlockHeight));
        uint256 n3 = vin.prevout.hash > n2 ? (vin.prevout.hash - n2) : (n2 - vin.prevout.hash);

        return n3;
    }

    IMPLEMENT_SERIALIZE(
        READWRITE(nBlockHeight);
        READWRITE(payee);
        READWRITE(vin);
        READWRITE(score);
        READWRITE(vchSig);
    )
};

//
// Masternode Payments Class
// Keeps track of who should get paid for which blocks
//

class CMasternodePayments
{
private:
    std::vector<CMasternodePaymentWinner> vWinning;
    int nSyncedFromPeer;
    std::string strMasterPrivKey;
    std::string strMainPubKey;
    std::string strTestPubKey;
    bool enabled;
    int nLastBlockHeight;

public:

    CMasternodePayments() {
        strMainPubKey = "044963f49ea0d59166067c690710d1b8c8f132b799f1789fd02159dac851c5046adfbde4adce8533b6e240b75c8cdb7887cc158c9a492852adcb5df1e64829a19a";
        strTestPubKey = "043a36231e775b5293f1f241352c591f2589d67b7db79890d16bd4c0bac21b49f38ffed5ffced6017fa7386710a6f55858b75b035d5567e47f0de78aa9b8d27ca2";
        enabled = false;
    }

    bool SetPrivKey(std::string strPrivKey);
    bool CheckSignature(CMasternodePaymentWinner& winner);
    bool Sign(CMasternodePaymentWinner& winner);

    // Deterministically calculate a given "score" for a masternode depending on how close it's hash is
    // to the blockHeight. The further away they are the better, the furthest will win the election
    // and get paid this block
    //

    uint64_t CalculateScore(uint256 blockHash, CTxIn& vin);
    bool GetWinningMasternode(int nBlockHeight, CTxIn& vinOut);
    bool AddWinningMasternode(CMasternodePaymentWinner& winner);
    bool ProcessBlock(int nBlockHeight);
    void Relay(CMasternodePaymentWinner& winner);
    void Sync(CNode* node);
    void CleanPaymentList();
    int LastPayment(CMasternode& mn);
    int GetMinMasternodePaymentsProto();

    bool GetBlockPayee(int nBlockHeight, CScript& payee, CTxIn& vin);
};


#endif
