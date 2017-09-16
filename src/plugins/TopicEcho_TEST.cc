/*
 * Copyright (C) 2017 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

#include <gtest/gtest.h>

#include <ignition/msgs.hh>
#include <ignition/transport/Node.hh>

#include "ignition/gui/Iface.hh"
#include "ignition/gui/Plugin.hh"

using namespace ignition;
using namespace gui;

/////////////////////////////////////////////////
TEST(TopicEchoTest, Load)
{
  EXPECT_TRUE(initApp());

  EXPECT_TRUE(loadPlugin("TopicEcho"));

  EXPECT_TRUE(stop());
}

/////////////////////////////////////////////////
TEST(TopicEchoTest, Echo)
{
  setVerbosity(4);
  EXPECT_TRUE(initApp());

  // Load plugin
  EXPECT_TRUE(loadPlugin("TopicEcho"));

  // Create main window
  EXPECT_TRUE(createMainWindow());
  auto win = mainWindow();
  EXPECT_TRUE(win != nullptr);

  // Get plugin
  auto plugins = win->findChildren<Plugin *>();
  EXPECT_EQ(plugins.size(), 1);
  auto plugin = plugins[0];
  EXPECT_EQ(plugin->Title(), "Topic echo");

  // Widgets
  auto echoButton = plugin->findChild<QPushButton *>("echoButton");
  EXPECT_TRUE(echoButton != nullptr);
  EXPECT_EQ(echoButton->text(), "Echo");

  auto topicEdit = plugin->findChild<QLineEdit *>("topicEdit");
  EXPECT_TRUE(topicEdit != nullptr);
  EXPECT_EQ(topicEdit->text(), "/echo");

  auto msgList = plugin->findChild<QListWidget *>("msgList");
  EXPECT_TRUE(msgList != nullptr);
  EXPECT_EQ(msgList->count(), 0);

  auto bufferSpin = plugin->findChild<QSpinBox *>("bufferSpin");
  EXPECT_TRUE(bufferSpin != nullptr);
  EXPECT_EQ(bufferSpin->value(), 10);

  auto pauseCheck = plugin->findChild<QCheckBox *>("pauseCheck");
  EXPECT_TRUE(pauseCheck != nullptr);
  EXPECT_FALSE(pauseCheck->isChecked());

  // Start echoing
  echoButton->click();
  EXPECT_EQ(echoButton->text(), "Stop echoing");

  // Publish string
  transport::Node node;
  auto pub = node.Advertise<msgs::StringMsg>("/echo");

  {
    msgs::StringMsg msg;
    msg.set_data("example string");
    pub.Publish(msg);
  }

  int sleep = 0;
  int maxSleep = 30;
  while (msgList->count() == 0 && sleep < maxSleep)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    QCoreApplication::processEvents();
    sleep++;
  }

  // Check message was echoed
  ASSERT_EQ(msgList->count(), 1);
  EXPECT_EQ(msgList->item(0)->text(), QString("data: \"example string\"\n"))
      << msgList->item(0)->text().toStdString();

  // Publish more than buffer size
  for (auto i = 0; i < bufferSpin->value() + 5; ++i)
  {
    msgs::StringMsg msg;
    msg.set_data("many messages: " + std::to_string(i));
    pub.Publish(msg);
  }

  sleep = 0;
  while (msgList->count() < 10 && sleep < maxSleep)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    QCoreApplication::processEvents();
    sleep++;
  }

  ASSERT_EQ(msgList->count(), 10);
  EXPECT_EQ(msgList->item(0)->text(), QString("data: \"many messages: 5\"\n"))
      << msgList->item(0)->text().toStdString();
  EXPECT_EQ(msgList->item(9)->text(), QString("data: \"many messages: 14\"\n"))
      << msgList->item(9)->text().toStdString();

  // Increase buffer
  bufferSpin->setValue(20);

  // Publish another message and now it fits
  {
    msgs::StringMsg msg;
    msg.set_data("new message");
    pub.Publish(msg);
  }

  sleep = 0;
  while (msgList->count() < 11 && sleep < maxSleep)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    QCoreApplication::processEvents();
    sleep++;
  }

  ASSERT_EQ(msgList->count(), 11);
  EXPECT_EQ(msgList->item(10)->text(), QString("data: \"new message\"\n"))
      << msgList->item(10)->text().toStdString();

  // Pause
  pauseCheck->click();

  // Publish another message and it is not received
  {
    msgs::StringMsg msg;
    msg.set_data("dropped message");
    pub.Publish(msg);
  }

  sleep = 0;
  while (msgList->count() < 11 && sleep < maxSleep)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    QCoreApplication::processEvents();
    sleep++;
  }

  ASSERT_EQ(msgList->count(), 11);
  EXPECT_EQ(msgList->item(10)->text(), QString("data: \"new message\"\n"))
      << msgList->item(10)->text().toStdString();

  // Decrease buffer
  bufferSpin->setValue(5);

  ASSERT_EQ(msgList->count(), 5);
  EXPECT_EQ(msgList->item(0)->text(), QString("data: \"many messages: 11\"\n"))
      << msgList->item(0)->text().toStdString();
  EXPECT_EQ(msgList->item(4)->text(), QString("data: \"new message\"\n"))
      << msgList->item(4)->text().toStdString();

  // Stop echoing
  echoButton->click();
  EXPECT_EQ(echoButton->text(), "Echo");
  ASSERT_EQ(msgList->count(), 0);

  sleep = 0;
  while (msgList->count() == 0 && sleep < maxSleep)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    QCoreApplication::processEvents();
    sleep++;
  }

  ASSERT_EQ(msgList->count(), 0);

  // Start echoing again
  echoButton->click();
  EXPECT_EQ(echoButton->text(), "Stop echoing");

  // Stop echoing by editing topic
  topicEdit->setText("/another_topic");
  EXPECT_EQ(echoButton->text(), "Echo");
  ASSERT_EQ(msgList->count(), 0);

  sleep = 0;
  while (msgList->count() == 0 && sleep < maxSleep)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    QCoreApplication::processEvents();
    sleep++;
  }

  ASSERT_EQ(msgList->count(), 0);

  EXPECT_TRUE(stop());
}
