# GWatchD - Watcher Daemon

## Description
GWatchD uses inotify to monitor file system events. If an event occur, it executes processes (called "job") assigned to directory in which inotify reported any change.

Each job is a plugin which has own config file located in `/etc/gwatchd/job` dir with name like `<job_name>.yml`, for example `synchronize.yml`. Daemon also has own config file: `/etc/gwatchd/config.yml`.

Logs can be found in `/var/log/gwatchd` and `/var/log/gwatchd/job` dirs. Daemon and plugins have separate log files (just like config files).

## Available jobs
- [**Synchronize**](https://github.com/gorzechowski/gwatchd/wiki/Synchronize-Job)

## Daemon configuration

Optional properties:

| Property                                  | Description                                |
| ----------------------------------------: | ------------------------------------------ |
| log.maxFileSize `<integer>`: 5120         | Max size of daemon log file in kilobytes   |
| log.dirPath `<string>`: /var/log/gwatchd  | Path where daemon and jobs logs are stored |
| lib.dirPath `<string>`: /usr/lib/gwatchd  | Path where daemon looks for `job` dir. <br /><br />Note that if You change this directory, application still looks in default directory for dependencies. You can add library path then using `LD_LIBRARY_PATH`. |

Example config:
```yaml
log:
  maxFileSize: 10000
  dirPath: /home/user/logs/gwatchd

lib:
  dirPath: /home/user/gwatchd
``` 

## License
[GPLv2](http://www.fsf.org/licensing/licenses/info/GPLv2.html) or any later GPL version.

## Disclaimer
Besides the usual disclaimer in the license, we want to specifically emphasize that neither the authors nor any organization the authors are associated with can and will hold responsible for data-loss caused by possible malfunctions of GWatchD.
