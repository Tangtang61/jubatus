// Jubatus: Online machine learning framework for distributed environment
// Copyright (C) 2013 Preferred Networks and Nippon Telegraph and Telephone Corporation.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License version 2.1 as published by the Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

#include <gtest/gtest.h>
#include <jubatus/client/classifier_client.hpp>
#include "util.hpp"
#include "status_test.hpp"

using std::vector;
using jubatus::classifier::estimate_result;
using jubatus::classifier::labeled_datum;
using jubatus::classifier::client::classifier;

namespace {
vector<labeled_datum> make_labeled_data() {
  jubatus::client::common::datum datum;
  vector<labeled_datum> data;
  data.push_back(labeled_datum("label", datum));
  return data;
}
vector<jubatus::client::common::datum> make_data() {
  vector<jubatus::client::common::datum> data;
  return data;
}
}

TEST(classifier_test, get_config) {
  classifier cli(host(), port(), cluster_name(), timeout());
  ASSERT_NE("", cli.get_config());
}

TEST(classifier_test, get_status) {
  classifier cli(host(), port(), cluster_name(), timeout());

  std::map<std::string, std::map<std::string, std::string> >
    res = cli.get_status();

  ASSERT_NE(0, res.size());

  std::map<std::string, std::map<std::string, std::string> >
      ::iterator it = res.begin();
  while(it != res.end()) {
    ASSERT_NE(0, it->second.size());
    ASSERT_NO_FATAL_FAILURE(assert_common_status(it->second));
    ASSERT_TRUE(has_key(it->second, "type"));
    ASSERT_EQ("classifier", it->second["type"]);
    ++it;
  }
}

TEST(classifier_test, save_load) {
  classifier cli(host(), port(), cluster_name(), timeout());
  ASSERT_EQ(1, cli.save("classifier.save_test.model").size());
  ASSERT_TRUE(cli.load("classifier.save_test.model"));
}

TEST(classifier_test, train) {
  classifier cli(host(), port(), cluster_name(), timeout());
  cli.train(make_labeled_data());
}

TEST(classifier_test, classify) {
  classifier cli(host(), port(), cluster_name(), timeout());
  vector<vector<estimate_result> >
    result = cli.classify(make_data());
  ASSERT_EQ(0, result.size());
}

TEST(classifier_test, clear) {
  classifier cli(host(), port(), cluster_name(), timeout());
  ASSERT_TRUE(cli.clear());
}
