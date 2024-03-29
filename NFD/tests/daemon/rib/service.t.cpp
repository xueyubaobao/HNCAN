/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2014-2019,  Regents of the University of California,
 *                           Arizona Board of Regents,
 *                           Colorado State University,
 *                           University Pierre & Marie Curie, Sorbonne University,
 *                           Washington University in St. Louis,
 *                           Beijing Institute of Technology,
 *                           The University of Memphis.
 *
 * This file is part of NFD (Named Data Networking Forwarding Daemon).
 * See AUTHORS.md for complete list of NFD authors and contributors.
 *
 * NFD is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * NFD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * NFD, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "rib/service.hpp"
#include "daemon/global.hpp"

#include "tests/test-common.hpp"
#include "tests/daemon/rib-io-fixture.hpp"

namespace nfd {
namespace rib {
namespace tests {

using namespace nfd::tests;

BOOST_FIXTURE_TEST_SUITE(TestService, RibIoFixture)

BOOST_AUTO_TEST_CASE(Basic)
{
  ConfigSection section;
  section.put("face_system.unix.path", "/var/run/nfd.sock");

  ndn::KeyChain ribKeyChain;

  BOOST_CHECK_THROW(Service::get(), std::logic_error);
  BOOST_CHECK_THROW(Service(section, ribKeyChain), std::logic_error);

  runOnRibIoService([&] {
    {
      BOOST_CHECK_THROW(Service::get(), std::logic_error);
      Service ribService(section, ribKeyChain);
      BOOST_CHECK_EQUAL(&ribService, &Service::get());
    }
    BOOST_CHECK_THROW(Service::get(), std::logic_error);
    Service ribService(section, ribKeyChain);
    BOOST_CHECK_THROW(Service(section, ribKeyChain), std::logic_error);
  });
}

BOOST_AUTO_TEST_SUITE_END() // TestRibService

} // namespace tests
} // namespace rib
} // namespace nfd
