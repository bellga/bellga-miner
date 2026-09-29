/* XMRig
 * Copyright (c) 2018-2022 SChernykh   <https://github.com/SChernykh>
 * Copyright (c) 2016-2022 XMRig       <https://github.com/xmrig>, <support@xmrig.com>
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef XMRIG_DONATE_H
#define XMRIG_DONATE_H


#include <cstdint>


/*
 * Dev donation (this fork).
 *
 * Percentage of hashing power donated to this fork's maintainer. The DEFAULT is 5%, but the
 * end user can lower it in their config (`donate-level`) or via `--donate-level`. A MINIMUM of
 * 1% is enforced and cannot be bypassed: if the user configures 0 or a negative value, the
 * minimum applies instead of disabling donation entirely (see Pools::setDonateLevel()).
 *
 * Example of how it works for the setting of 1%:
 * Your miner will mine into your usual pool for a random time (in a range from 49.5 to 148.5 minutes),
 * then switch to the developer's pool for 1 minute, then switch again to your pool for 99 minutes
 * and then switch again to developer's pool for 1 minute; these rounds will continue until the miner stops.
 *
 * Randomised only on the first round to prevent waves on the donation pool.
 *
 * Switching is instant and only happens after a successful connection, so you never lose any hashes.
 */
constexpr const int kDefaultDonateLevel = 5;
constexpr const int kMinimumDonateLevel = 1;


/*
 * Donation routing (this fork, bellga.tech era).
 *
 * Three coins are mined on our own pool (pool.bellga.tech, miningcore): VRSC (vrsc1, port 3960),
 * ZEPH (zeph1, port 3961) and XMR (xmr1, port 3962). For those three, donation now goes to OUR
 * OWN pool, paid in the SAME coin the user is actually mining -- see DonateStrategy.cpp's
 * activeStrategy(), which picks the matching dedicated pool below.
 *
 * VRSC is unambiguous: it has its own algorithm family (Algorithm::VERUSHASH), so
 * activeStrategy() can key off the algorithm alone.
 *
 * ZEPH and XMR are NOT distinguishable by algorithm -- Zephyr mines with plain RandomX (family
 * RANDOM_X, same "rx/0" as Monero; this fork has no separate algorithm id for it, see
 * Algorithm.h). So activeStrategy() disambiguates those two by which of our own pool's stratum
 * endpoints (host+port) the miner is actually connected to, not by algorithm. A RandomX-family
 * job on any OTHER pool (not pool.bellga.tech:3961/3962) falls through to the generic
 * MoneroOcean pool below -- we have no way to know what coin a third-party RandomX pool pays in.
 *
 * kDonateWalletGeneric is used for the existing MoneroOcean multi-algo donation pool
 * (xmrig.moneroocean.stream). This remains the fallback for every algorithm this fork supports
 * that ISN'T one of our own three pool coins above (KawPow/RVN, GhostRider/RTM, other
 * CryptoNight coins, RandomX on a pool other than ours, ...) -- standing up our own
 * daemon+wallet-rpc+pool just to receive a sliver of donation hashpower for those isn't worth
 * the infrastructure, so they keep going through MoneroOcean's pool, just paid to our own
 * wallet now (an XMR address) instead of the placeholder that shipped here before.
 */
constexpr const char *kDonateWalletGeneric = "49fnZSYNtodT6b533TbDjHEd8qRPzB9jpX5Z86DjSM3RL6Ytu171KGzegqdPR63xnvH5mtzgby5MwQsLAz61zUa3NsMRW9g";

// Our own pool host, shared by the three dedicated donation pools below.
constexpr const char *kDonatePoolHost      = "pool.bellga.tech";

// VRSC (vrsc1) -- ".SsA15" is a worker/rig name (per luckpool's convention this address was
// originally used with), kept so donation traffic shows as its own worker on the dashboard.
constexpr const char *kDonateWalletVerus   = "RQrN3fm7tgNoSHJ1Beu9YQ3Ds3kgPo47Vu.SsA15";
constexpr const char *kDonateHostVerus     = kDonatePoolHost;
constexpr const uint16_t kDonatePortVerus  = 3960;

// ZEPH (zeph1) -- reuses the pool's own fee wallet address (user's explicit choice; mixes fee
// and donation income in the same wallet, simpler than minting a dedicated one).
constexpr const char *kDonateWalletZeph    = "ZEPHYR3c37E83N46MoFbH8KVsz1vhLCYSAEPVc2M5DAdfuZL6Mjxvdcg6WsGtBG3pnEJp5yf4tcdJ58pCYxE7RAQG7aVoywQhuf4d";
constexpr const char *kDonateHostZeph      = kDonatePoolHost;
constexpr const uint16_t kDonatePortZeph   = 3961;

// XMR (xmr1) -- pool not necessarily live yet (depends on monerod sync); wired up in advance so
// donation starts working the moment xmr1 comes online, no code change needed then.
constexpr const char *kDonateWalletXmr     = "49fnZSYNtodT6b533TbDjHEd8qRPzB9jpX5Z86DjSM3RL6Ytu171KGzegqdPR63xnvH5mtzgby5MwQsLAz61zUa3NsMRW9g";
constexpr const char *kDonateHostXmr       = kDonatePoolHost;
constexpr const uint16_t kDonatePortXmr    = 3962;


#endif // XMRIG_DONATE_H
