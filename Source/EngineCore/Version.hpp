// Copyright 2020, Nathan Blane

#pragma once

#include "Types/Intrinsics.hpp"

#define MUSA_VERSION(major, minor, patch) \
			((major << 24) | ((minor & 0xff) << 16) | (patch))

constexpr uint32 MusaVersion = MUSA_VERSION(0, 0, 0);
constexpr uint32 MusaVersionMajor = (MusaVersion >> 24);
constexpr uint32 MusaVersionMinor = ((MusaVersion >> 16) & 0xff);
constexpr uint32 MusaVersionPatch = (MusaVersion & 0xffff);

constexpr uint32 MusaImporterVersion = MUSA_VERSION(0, 0, 1);