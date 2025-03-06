/*
 * Copyright (C) Canonical, Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "mock_server_reader_writer.h"
#include "stub_logger.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <multipass/logging/client_logger.h>
#include <multipass/logging/level.h>
#include <multipass/logging/multiplexing_logger.h>

namespace mpl = multipass::logging;
namespace mpt = multipass::test;

struct StubReply
{
    template <typename... Args>
    void set_log_line(Args&&...)
    {
    }
};

using uut_t = mpl::ClientLogger<StubReply, StubReply>;

struct client_logger_tests : ::testing::Test
{
    mpl::MultiplexingLogger stub_multiplexing_logger{std::make_unique<mpt::StubLogger>()};
    mpt::MockServerReaderWriter<StubReply, StubReply> mock_srw;
};

TEST_F(client_logger_tests, test_levels_as_string)
{
    EXPECT_CALL(mock_srw, Write(testing::_, testing::_)).Times(1);
    uut_t logger{mpl::Level::debug, stub_multiplexing_logger, &mock_srw};
    logger.log(mpl::Level::debug, "cat", "msg");
}
