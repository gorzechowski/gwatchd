[![Build Status](https://travis-ci.org/gorzechowski/gwatchd.svg?branch=master)](https://travis-ci.org/gorzechowski/gwatchd)

# GWatchD - Watcher Daemon

## Description
GWatchD uses inotify/kqueue to monitor file system events. If an event occur, it executes processes (called "job") assigned to directory in which watcher reported any change.

Each job is a plugin which has own config file located in `config/job` dir with name like `<job_name>.json`, for example `synchronize.json`. Daemon also has own config file: `config/config.json`.

Logs can be found in `logs` and `logs/job` dirs. Daemon and plugins have separate log files (just like config files).

## Available jobs
- [**Command**](https://github.com/gorzechowski/gwatchd/blob/master/docs/command-job.md)
- [**Synchronize**](https://github.com/gorzechowski/gwatchd/blob/master/docs/synchronize-job.md)

## Daemon configuration

Optional properties:

| Property                                  | Description                                |
| ----------------------------------------: | ------------------------------------------ |
| log.maxFileSize `<integer>`: 5120         | Max size of daemon log file in kilobytes   |
| log.dirPath `<string>`: logs              | Path where daemon and jobs logs are stored |
| socket.address `<string>`                 | Websocket server listen address. If empty, server will not be initialized. |
| socket.port `<integer>`                   | Websocket server listen port. If empty, server will not be initialized. |

Example config:
```json
{
    "log": {
        "maxFileSize": 10000,
        "dirPath": "/home/user/logs/gwatchd"
    },
    "socket": {
        "address": "127.0.0.1",
        "port": 8080
    }
}
```

## License
[GPLv2](http://www.fsf.org/licensing/licenses/info/GPLv2.html) or any later GPL version.

## Disclaimer
Besides the usual disclaimer in the license, we want to specifically emphasize that neither the authors nor any organization the authors are associated with can and will hold responsible for data-loss caused by possible malfunctions of GWatchD.
