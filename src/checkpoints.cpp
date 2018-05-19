// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/foreach.hpp>

#include "checkpoints.h"

#include "txdb.h"
#include "main.h"
#include "uint256.h"


static const int nCheckpointSpan = 5000;

namespace Checkpoints
{
    typedef std::map<int, uint256> MapCheckpoints;

    //
    // What makes a good checkpoint block?
    // + Is surrounded by blocks with reasonable timestamps
    //   (no blocks before with a timestamp after, none after with
    //    timestamp before)
    // + Contains no strange transactions
    //
    static MapCheckpoints mapCheckpoints =
        boost::assign::map_list_of
        (1, uint256("0x00000b7ff05d5ef83c0b524732ab2c01b0505d3381d7505169d90f061fd47866"))
        (3, uint256("0x000000c5823bdc4409c6882b39a8be7c113af492fb297818d37ddcf2d4d1e0c8"))
		(10000, uint256("0x00000000003457e07d2b7de1c902203947c8d0ded423dcede6acbe62c04e7136"))
		(20000, uint256("0x0000000000088ba3704db742c03d181f5672f1e1a9e2aedcb6e0ed5f24410a09"))
		(30000, uint256("0x0000000000427730ce684c1a830dae58e3fc9a0bf62d4efd820974102d77fc94"))
		(40000, uint256("0x0000000000161db819c780310eef69380a23ed53b4b36eab615618ef2a7c5947"))
		(50000, uint256("0x000000000056c64bab4b08c81196775e8489627e265a93842c8c6690c214346b"))
		(60000, uint256("0x000000000193c0e5b615abb6084fa0e627172c5f17f697df6285d8428b82aa3c"))
		(70000, uint256("0x00000000013986ae19f691b10ca60e49b5221c10640c18414fe7bd3a0ddcb497"))
		(75000, uint256("0x0000000008ef2127de6b89fe3e608ee40d079f5629876aafb353e9bf5f87f556"))
		(80000, uint256("0x000000000142fac657230cb0ffd91e3dfa5693742ee4074f996b87f3cad7dbab"))
		(90000, uint256("0x0000000007c6124908398129355408c46bd01e0cacd43bb0862cdfeee136453b"))
		(100000, uint256("0x00000000007ce264c00521645adc9ccb627cae68f188c4f68a895d5982607386"))
	    (120000, uint256("0x0000000001f3aca90538662cc64a37eb085861a554665f7bf7cb283ad7c8e571"))
	    (145000, uint256("0x0000000000a445584b8ddfd106c0ba706318c6a7e43168cfcf53720ef07c3fcf"))
        ;

    // TestNet has no checkpoints
    static MapCheckpoints mapCheckpointsTestnet =
        boost::assign::map_list_of
        (     0, uint256("0x0000082f5939c2154dbcba35f784530d12e9d72472fcfaf29674ea312cdf4c83"))
        (  5000, uint256("0x00000013fe8e170075b47b83447a73e7ecd0a3ae0c199aa6427a41437134e31a"))
        ( 10000, uint256("0x000000c541d1903e7b8441397d9bda5a1d4eedfe4c4a8aea38446814740752d4"))
        ( 12000, uint256("0x000000df243a71405ced83a1fe32a645d42e7497d5d06b9d57d12fb25d050389"))
        ;

    bool CheckHardened(int nHeight, const uint256& hash)
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        MapCheckpoints::const_iterator i = checkpoints.find(nHeight);
        if (i == checkpoints.end()) return true;
        return hash == i->second;
    }

    int GetTotalBlocksEstimate()
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        if (checkpoints.empty())
            return 0;
        return checkpoints.rbegin()->first;
    }

    CBlockIndex* GetLastCheckpoint(const std::map<uint256, CBlockIndex*>& mapBlockIndex)
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        BOOST_REVERSE_FOREACH(const MapCheckpoints::value_type& i, checkpoints)
        {
            const uint256& hash = i.second;
            std::map<uint256, CBlockIndex*>::const_iterator t = mapBlockIndex.find(hash);
            if (t != mapBlockIndex.end())
                return t->second;
        }
        return NULL;
    }

    // Automatically select a suitable sync-checkpoint
    const CBlockIndex* AutoSelectSyncCheckpoint()
    {
        const CBlockIndex *pindex = pindexBest;
        // Search backward for a block within max span and maturity window
        while (pindex->pprev && pindex->nHeight + nCheckpointSpan > pindexBest->nHeight)
            pindex = pindex->pprev;
        return pindex;
    }

    // Check against synchronized checkpoint
    bool CheckSync(int nHeight)
    {
        const CBlockIndex* pindexSync = AutoSelectSyncCheckpoint();
        if (nHeight <= pindexSync->nHeight){
            return false;
        }
        return true;
    }
}
