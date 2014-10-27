#pragma once

using Entity = sses::Entity;
using Component = sses::Component;
using World = ssvsc::World<ssvsc::HashGrid, ssvsc::Impulse>;
using Body = World::BodyType;
using Sensor = World::SensorType;
using DetectionInfo = World::DetectionInfoType;
using ResolutionInfo = World::ResolutionInfoType;