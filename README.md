Android-AppDaemon
=================

App process daemon, used to keep your app alive.

Usage
=====
* Copy the `daemon` executable file into assets, then use `Daemon.run(context, daemonServiceClazz, intervalTime)` to run the daemon.
* You need to add `android:exported="true"` to your daemon service in manifest so that daemon can start up your daemon service.

License
=======

    Copyright (C) 2015 Vincent Cheung

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

         http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.