/*
 * Copyright (C) 2021 The Android Open Source Project
 * Android BPF library - public API
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <log/log.h>

#include <android-base/properties.h>

namespace android {
namespace bpf {

// Wait for bpfloader to load BPF programs.
// BPF-less devices (old kernels): the property is never set, bound the total
// wait to 10s and proceed so that lmkd/netd can fall back to legacy paths
// instead of hanging boot forever.
static inline void waitForProgsLoaded() {
    // bounded: 5+5s, then give up
    for (int delay = 5; delay <= 10; delay *= 2) {
        if (android::base::WaitForProperty("bpf.progs_loaded", "1", std::chrono::seconds(delay)))
            return;
        ALOGW("Waited %ds for bpf.progs_loaded, still waiting...", delay);
    }
    ALOGE("bpf.progs_loaded not set after 10s (BPF-less kernel?); continuing without BPF");
}

}  // namespace bpf
}  // namespace android
