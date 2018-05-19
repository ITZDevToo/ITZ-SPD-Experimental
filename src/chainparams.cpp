// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2012 The Bitcoin developers
// Copyright (c) 2014-2015 Dash Developers
// Copyright (c) 2017-2018 The Stipend developers
// Copyright (c) 2017-2018 The Interzone developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "assert.h"
#include "chainparams.h"
#include "main.h"
#include "util.h"
#include "base58.h"
#include "protocol.h"
#include "core.h"
#include <boost/assign/list_of.hpp>

using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

//
// Main network
//

// Convert the pnSeeds array into usable address objects.
static void convertSeeds(std::vector<CAddress> &vSeedsOut, const unsigned int *data, unsigned int count, int port)
{
     // It'll only connect to one or two seed nodes because once it connects,
     // it'll get a pile of addresses with newer timestamps.
     // Seed nodes are given a random 'last seen time' of between one and two
     // weeks ago.
     const int64_t nOneWeek = 7*24*60*60;
    for (unsigned int k = 0; k < count; ++k)
    {
        struct in_addr ip;
        unsigned int i = data[k], t;

        // -- convert to big endian
        t =   (i & 0x000000ff) << 24u
            | (i & 0x0000ff00) << 8u
            | (i & 0x00ff0000) >> 8u
            | (i & 0xff000000) >> 24u;

        memcpy(&ip, &t, sizeof(ip));

        CAddress addr(CService(ip, port));
        addr.nTime = GetTime()-GetRand(nOneWeek)-nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

// Hardcoded seeds.
static void getHardcodedSeeds(std::vector<CAddress> &vSeedsOut)
{
    std::vector<std::string> ips;
    ips.push_back("188.226.139.237");
    ips.push_back("104.207.133.227");
    ips.push_back("45.63.22.217");

    const int64_t oneWeek = 7 * 24 * 60 * 60;
    for (size_t i = 0; i < ips.size(); ++i)
    {
        CAddress addr(CService(ips[i], 55675));
        addr.nTime = GetTime() - GetRand(oneWeek) - oneWeek;
        vSeedsOut.push_back(addr);
    }
}

class CMainParams : public CChainParams {
public:
    CMainParams() {
        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        pchMessageStart[0] = 0x08;
        pchMessageStart[1] = 0x02;
        pchMessageStart[2] = 0x01;
        pchMessageStart[3] = 0x17;
        vAlertPubKey = ParseHex("04579f18934b3ef39094a9999e45506a1935662d0cd4e504d07beb53b8a1bfd78d81bee47e65119318397809420d5320e3c7b2aaae58580db48c38a4e6d4f0f919");
        nDefaultPort = 55675;
        nRPCPort = 55680;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 20);
        nSubsidyHalvingInterval = 500000; 

        
        const char* pszTimestamp = "The rulers of this most insecure of all worlds are rulers by accident.";
        std::vector<CTxIn> vin;
        vin.resize(1);
        vin[0].scriptSig = CScript() << 486604799 << CBigNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        std::vector<CTxOut> vout;
        vout.resize(1);
        //vout[0].SetEmpty();
        vout[0].nValue = 16 * COIN;
        vout[0].scriptPubKey = CScript() << ParseHex("04becedf6ebadd4596964d890f677f8d2e74fdcc313c6416434384a66d6d8758d1c92de272dc6713e4a81d98841dfdfdc95e204ba915447d2fe9313435c78af3e8") << OP_CHECKSIG;

        CTransaction txNew(1, 1503332100, vin, vout, 0);
        

         // uncomment to log genesis block info        
      //  start
        if (true && genesis.GetHash() != hashGenesisBlock)
                       {
                           printf("Searching for genesis block...\n");
                          uint256 hashTarget = CBigNum().SetCompact(genesis.nBits).getuint256();
                          uint256 thash;
                         /*while (true)
                          {
                              thash = genesis.GetHash();
                              if (thash <= hashTarget)
                                  break;
                              if ((genesis.nNonce & 0xFFF) == 0)
                              {
                                  printf("nonce %08X: hash = %s (target = %s)\n", genesis.nNonce, thash.ToString().c_str(), hashTarget.ToString().c_str());
                              }
                              ++genesis.nNonce;
                              if (genesis.nNonce == 0)
                              {
                                  printf("NONCE WRAPPED, incrementing time\n");
                                  ++genesis.nTime;
                              }
                          }
                          */
                          printf("genesis.pszTimestamp = %s \n", pszTimestamp);
                          printf("genesis.nBits = %u \n", genesis.nBits);
                          printf("genesis.nTime = %u \n", genesis.nTime);
                          printf("genesis.nNonce = %u \n", genesis.nNonce);
                          printf("genesis.nVersion = %u \n", genesis.nVersion);
                          printf("genesis.GetHash = %s\n", genesis.GetHash().ToString().c_str()); //first this, then comment this line out and uncomment the one under.
                          printf("genesis.hashMerkleRoot = %s \n", genesis.hashMerkleRoot.ToString().c_str()); //improvised. worked for me, to find merkle root

                      }

//end

        /*
        // Genesis block
        const char* pszTimestamp = "The rulers of this most insecure of all worlds are rulers by accident.";
        CTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CBigNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 16 * COIN;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("04becedf6ebadd4596964d890f677f8d2e74fdcc313c6416434384a66d6d8758d1c92de272dc6713e4a81d98841dfdfdc95e204ba915447d2fe9313435c78af3e8") << OP_CHECKSIG;
        */

        LogPrintf("genesis mainnet transaction:  %s\n", txNew.ToString().c_str());

        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime    = 1503332100;
        genesis.nBits    = 0x1e0fffff;
        //genesis.nBits    = bnProofOfWorkLimit.GetCompact();
        genesis.nNonce   = 294622;

  // uncomment to log genesis block info        
      //  start
        if (true && genesis.GetHash() != hashGenesisBlock)
                       {
                           printf("Searching for genesis block...\n");
                          uint256 hashTarget = CBigNum().SetCompact(genesis.nBits).getuint256();
                          uint256 thash;
                         /*while (true)
                          {
                              thash = genesis.GetHash();
                              if (thash <= hashTarget)
                                  break;
                              if ((genesis.nNonce & 0xFFF) == 0)
                              {
                                  printf("nonce %08X: hash = %s (target = %s)\n", genesis.nNonce, thash.ToString().c_str(), hashTarget.ToString().c_str());
                              }
                              ++genesis.nNonce;
                              if (genesis.nNonce == 0)
                              {
                                  printf("NONCE WRAPPED, incrementing time\n");
                                  ++genesis.nTime;
                              }
                          }
                          */
                          printf("genesis.pszTimestamp = %s \n", pszTimestamp);
                          printf("genesis.nBits = %u \n", genesis.nBits);
                          printf("genesis.nTime = %u \n", genesis.nTime);
                          printf("genesis.nNonce = %u \n", genesis.nNonce);
                          printf("genesis.nVersion = %u \n", genesis.nVersion);
                          printf("genesis.GetHash = %s\n", genesis.GetHash().ToString().c_str()); //first this, then comment this line out and uncomment the one under.
                          printf("genesis.hashMerkleRoot = %s \n", genesis.hashMerkleRoot.ToString().c_str()); //improvised. worked for me, to find merkle root

                      }

//end

        hashGenesisBlock = genesis.GetHash();

        assert(hashGenesisBlock == uint256("0x00000a3e2f4ab22bae06eb1d3121262055fef0fc03da5337b1c7c153af133780"));
        assert(genesis.hashMerkleRoot == uint256("0x11f8d6fdf5460c2308b76da69942dd9fa386f9d337fbc95f669d73bcf02b64a5"));


        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,0);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,5);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,28 + 128);  
        //base58Prefixes[STEALTH_ADDRESS] = std::vector<unsigned char>(1,41);
        //base58Prefixes[EXT_PUBLIC_KEY] = list_of(0x99)(0x75)(0x45)(0xE2).convert_to_container<std::vector<unsigned char> >();
        //base58Prefixes[EXT_SECRET_KEY] = list_of(0x99)(0x73)(0x43)(0xE3).convert_to_container<std::vector<unsigned char> >();

	// NOTE: MODIFIED THIS TO 0.0.0.0
    vSeeds.push_back(CDNSSeedData("1",  "188.226.139.237"));
	vSeeds.push_back(CDNSSeedData("2",  "104.207.133.227"));
	vSeeds.push_back(CDNSSeedData("3",  "45.63.22.217"));
	//vSeeds.push_back(CDNSSeedData("4",  "34.231.228.73"));
	//vSeeds.push_back(CDNSSeedData("5",  "34.239.99.171"));
	//vSeeds.push_back(CDNSSeedData("6",  "18.218.0.160"));
	//vSeeds.push_back(CDNSSeedData("7",  "13.59.189.252"));
	//vSeeds.push_back(CDNSSeedData("8",  "52.15.218.190"));

	convertSeeds(vFixedSeeds, pnSeed, ARRAYLEN(pnSeed), nDefaultPort);

        getHardcodedSeeds(vFixedSeeds);

        nPoolMaxTransactions = 3;
        strDarksendPoolDummyAddress = "175M2dV2DaEJWcTjxrGPJT88WDUE7Bj44U";

        nLastPOWBlock = 450000;
        nPOSStartBlock = 210000;
    }

    virtual const CBlock& GenesisBlock() const { return genesis; }
    virtual Network NetworkID() const { return CChainParams::MAIN; }

    virtual const vector<CAddress>& FixedSeeds() const {
        return vFixedSeeds;
    }
protected:
    CBlock genesis;
    vector<CAddress> vFixedSeeds;
};
static CMainParams mainParams;


//
// Testnet
//

class CTestNetParams : public CMainParams {
public:
    CTestNetParams() {
        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        pchMessageStart[0] = 0xfb;
        pchMessageStart[1] = 0xc2;
        pchMessageStart[2] = 0x11;
        pchMessageStart[3] = 0x02;
        //bnProofOfWorkLimit = CBigNum(~uint256(0) >> 20);
        vAlertPubKey = ParseHex("04c4d925d462a89054155314a7068bb8cf6d7c030d76491e6cc97fefcc4ab8c3665bfb34c3aba0e12b9fb4fab47502a00938dd93e0e675c4713ee47200046de2e3");
        nDefaultPort = 21817;
        nRPCPort = 21818;
        strDataDir = "testnet";
        genesis.nTime    = 1503324223;
        //genesis.nBits    = 0x207fffff();
        genesis.nNonce   = 1088722;

        hashGenesisBlock = genesis.GetHash();

        LogPrintf("Display genesis hash so we can input it below %s\n", hashGenesisBlock.ToString().c_str());
	      LogPrintf("Display merkle root so we can input it below %s\n", genesis.hashMerkleRoot.ToString().c_str());

        assert(hashGenesisBlock == uint256("0x0000016920f7856a2a4f342db39b70591c00a699f1fe865ebc687fd9b66aa586"));

        vFixedSeeds.clear();
        vSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,102);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,44);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,88 + 128);
        //base58Prefixes[STEALTH_ADDRESS] = std::vector<unsigned char>(1,43);
        //base58Prefixes[EXT_PUBLIC_KEY] = list_of(0x98)(0x74)(0x44)(0xE1).convert_to_container<std::vector<unsigned char> >();
        //base58Prefixes[EXT_SECRET_KEY] = list_of(0x98)(0x72)(0x42)(0xE2).convert_to_container<std::vector<unsigned char> >();

        convertSeeds(vFixedSeeds, pnTestnetSeed, ARRAYLEN(pnTestnetSeed), nDefaultPort);

        nLastPOWBlock = 210000;
    }
    virtual Network NetworkID() const { return CChainParams::TESTNET; }
};
static CTestNetParams testNetParams;


static CChainParams *pCurrentParams = &mainParams;

const CChainParams &Params() {
    return *pCurrentParams;
}

void SelectParams(CChainParams::Network network) {
    switch (network) {
        case CChainParams::MAIN:
            pCurrentParams = &mainParams;
            break;
        case CChainParams::TESTNET:
            pCurrentParams = &testNetParams;
            break;
        default:
            assert(false && "Unimplemented network");
            return;
    }
}

bool SelectParamsFromCommandLine() {

    bool fTestNet = GetBoolArg("-testnet", false);

    if (fTestNet) {
        SelectParams(CChainParams::TESTNET);
    } else {
        SelectParams(CChainParams::MAIN);
    }
    return true;
}
