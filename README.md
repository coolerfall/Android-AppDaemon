Android-AppDaemon
=================

App process daemon, used to keep your app alive.

Usage
=====
* This daemon can be added in application or service in your app, use `Daemon.run(context, daemonServiceClazz, intervalTime)` to run the daemon, then you can do something in onStartCommand of daemon service.
* You need to add `android:exported="true"` to your daemon service in manifest so that daemon can start up your daemon service.
* If you want to monitor the uninstall of app, see also [Android-AppUninstallWatcher][1].

Note
====
This library dosen't work on all phones, such as xiaomi phones.

Download
========
If you are building with Gradle, simply add the following line to the dependencies section of your build.gradle file:

    compile 'com.coolerfall:android-app-daemon:1.2.0'

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

[1]: https://github.com/Coolerfall/Android-AppUninstallWatcher