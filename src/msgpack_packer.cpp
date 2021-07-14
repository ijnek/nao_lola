// Copyright 2021 Kenji Brameld
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <string>
#include <map>
#include <utility>
#include <memory>
#include <vector>
#include "nao_lola/msgpack_packer.hpp"
#include "msgpack.hpp"
#include "nao_lola/lola_enums.hpp"
#include "nao_lola/index_conversion.hpp"
#include "rclcpp/rclcpp.hpp"

std::string MsgpackPacker::getPacked()
{
  msgpack::zone z;
  std::map<std::string, msgpack::v2::object> map;

  if (position) {
    map.insert(std::make_pair("Position", msgpack::v2::object(position, z)));
  }

  if (stiffness) {
    map.insert(std::make_pair("Stiffness", msgpack::v2::object(stiffness, z)));
  }

  if (chest) {
    map.insert(std::make_pair("Chest", msgpack::v2::object(chest, z)));
  }

  if (l_ear) {
    map.insert(std::make_pair("LEar", msgpack::v2::object(l_ear, z)));
  }

  if (r_ear) {
    map.insert(std::make_pair("REar", msgpack::v2::object(r_ear, z)));
  }

  if (l_eye) {
    map.insert(std::make_pair("LEye", msgpack::v2::object(l_eye, z)));
  }

  if (r_eye) {
    map.insert(std::make_pair("REye", msgpack::v2::object(r_eye, z)));
  }

  if (l_foot) {
    map.insert(std::make_pair("LFoot", msgpack::v2::object(l_foot, z)));
  }

  if (r_foot) {
    map.insert(std::make_pair("RFoot", msgpack::v2::object(r_foot, z)));
  }

  if (skull) {
    map.insert(std::make_pair("Skull", msgpack::v2::object(skull, z)));
  }

  if (sonar) {
    map.insert(std::make_pair("Sonar", msgpack::v2::object(sonar, z)));
  }

  std::stringstream buffer;
  msgpack::pack(buffer, map);
  std::string packed = buffer.str();

  return packed;
}

void MsgpackPacker::setJointPositions(
  std::shared_ptr<nao_interfaces::msg::JointPositions> jointPositions)
{
  if (jointPositions->indexes.size() != jointPositions->positions.size()) {
    RCLCPP_ERROR(
      logger,
      "Incorrect message received for nao_interfaces::msg::JointPositions. "
      "Angles and Indexes vector must have the same length. "
      "Angles vector has length %zu, while indexes vector has length %zu",
      jointPositions->positions.size(), jointPositions->indexes.size());
  }

  if (!position) {
    position = std::make_shared<std::vector<float>>(static_cast<int>(LolaEnums::Joint::NUM_JOINTS));
  }

  for (unsigned i = 0; i < jointPositions->indexes.size(); ++i) {
    int msg_joint_index = jointPositions->indexes[i];
    float joint_angle = jointPositions->positions[i];
    LolaEnums::Joint lola_joint_index = IndexConversion::joint_msg_to_lola.at(msg_joint_index);
    position->at(static_cast<int>(lola_joint_index)) = joint_angle;
  }
}

void MsgpackPacker::setJointStiffnesses(
  std::shared_ptr<nao_interfaces::msg::JointStiffnesses> jointStiffnesses)
{
  if (jointStiffnesses->indexes.size() != jointStiffnesses->stiffnesses.size()) {
    RCLCPP_ERROR(
      logger,
      "Incorrect message received for nao_interfaces::msg::JointStiffnesses. "
      "Stiffnesses and Indexes vector must have the same length. "
      "Stiffnesses vector has length %zu, while indexes vector has length %zu",
      jointStiffnesses->stiffnesses.size(), jointStiffnesses->indexes.size());
  }

  if (!stiffness) {
    stiffness =
      std::make_shared<std::vector<float>>(static_cast<int>(LolaEnums::Joint::NUM_JOINTS));
  }

  for (unsigned i = 0; i < jointStiffnesses->indexes.size(); ++i) {
    int msg_joint_index = jointStiffnesses->indexes[i];
    float joint_stiffness = jointStiffnesses->stiffnesses[i];
    LolaEnums::Joint lola_joint_index = IndexConversion::joint_msg_to_lola.at(msg_joint_index);
    stiffness->at(static_cast<int>(lola_joint_index)) = joint_stiffness;
  }
}

void MsgpackPacker::setChestLed(std::shared_ptr<nao_interfaces::msg::ChestLed> chestLed)
{
  chest = std::make_shared<std::vector<float>>(3);
  chest->at(0) = chestLed->color.r;
  chest->at(1) = chestLed->color.g;
  chest->at(2) = chestLed->color.b;
}

void MsgpackPacker::setLeftEarLeds(std::shared_ptr<nao_interfaces::msg::LeftEarLeds> leftEarLeds)
{
  l_ear = std::make_shared<std::vector<float>>(10);
  for (unsigned i = 0; i < leftEarLeds->NUM_LEDS; ++i) {
    LolaEnums::LeftEarLeds lola_index = IndexConversion::left_ear_leds_msg_to_lola.at(i);
    l_ear->at(static_cast<int>(lola_index)) = leftEarLeds->intensities[i];
  }
}

void MsgpackPacker::setRightEarLeds(std::shared_ptr<nao_interfaces::msg::RightEarLeds> rightEarLeds)
{
  r_ear = std::make_shared<std::vector<float>>(10);
  for (unsigned i = 0; i < rightEarLeds->NUM_LEDS; ++i) {
    LolaEnums::RightEarLeds lola_index = IndexConversion::right_ear_leds_msg_to_lola.at(i);
    r_ear->at(static_cast<int>(lola_index)) = rightEarLeds->intensities[i];
  }
}

void MsgpackPacker::setLeftEyeLeds(std::shared_ptr<nao_interfaces::msg::LeftEyeLeds> leftEyeLeds)
{
  l_eye = std::make_shared<std::vector<float>>(24);
  for (unsigned i = 0; i < leftEyeLeds->NUM_LEDS; ++i) {
    LolaEnums::LeftEyeLeds lola_index = IndexConversion::left_eye_leds_msg_to_lola.at(i);
    l_eye->at(static_cast<int>(lola_index)) = leftEyeLeds->colors[i].r;
    l_eye->at(static_cast<int>(lola_index) + 8) = leftEyeLeds->colors[i].g;
    l_eye->at(static_cast<int>(lola_index) + 16) = leftEyeLeds->colors[i].b;
  }
}

void MsgpackPacker::setRightEyeLeds(std::shared_ptr<nao_interfaces::msg::RightEyeLeds> rightEyeLeds)
{
  r_eye = std::make_shared<std::vector<float>>(24);
  for (unsigned i = 0; i < rightEyeLeds->NUM_LEDS; ++i) {
    LolaEnums::RightEyeLeds lola_index = IndexConversion::right_eye_leds_msg_to_lola.at(i);
    r_eye->at(static_cast<int>(lola_index)) = rightEyeLeds->colors[i].r;
    r_eye->at(static_cast<int>(lola_index) + 8) = rightEyeLeds->colors[i].g;
    r_eye->at(static_cast<int>(lola_index) + 16) = rightEyeLeds->colors[i].b;
  }
}

void MsgpackPacker::setLeftFootLed(std::shared_ptr<nao_interfaces::msg::LeftFootLed> leftFootLed)
{
  l_foot = std::make_shared<std::vector<float>>(3);
  l_foot->at(0) = leftFootLed->color.r;
  l_foot->at(1) = leftFootLed->color.g;
  l_foot->at(2) = leftFootLed->color.b;
}

void MsgpackPacker::setRightFootLed(std::shared_ptr<nao_interfaces::msg::RightFootLed> rightFootLed)
{
  r_foot = std::make_shared<std::vector<float>>(3);
  r_foot->at(0) = rightFootLed->color.r;
  r_foot->at(1) = rightFootLed->color.g;
  r_foot->at(2) = rightFootLed->color.b;
}

void MsgpackPacker::setHeadLeds(std::shared_ptr<nao_interfaces::msg::HeadLeds> headLeds)
{
  skull = std::make_shared<std::vector<float>>(12);
  for (unsigned i = 0; i < headLeds->NUM_LEDS; ++i) {
    LolaEnums::SkullLeds lola_index = IndexConversion::head_leds_msg_to_lola.at(i);
    skull->at(static_cast<int>(lola_index)) = headLeds->intensities[i];
  }
}

void MsgpackPacker::setSonarUsage(std::shared_ptr<nao_interfaces::msg::SonarUsage> sonarUsage)
{
  sonar = std::make_shared<std::vector<bool>>(2);
  sonar->at(0) = sonarUsage->left;
  sonar->at(1) = sonarUsage->right;
}
