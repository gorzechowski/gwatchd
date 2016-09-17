# Synchronize Job

## Description
This job synchronizes remote directory with local directory using [Rsync](http://rsync.samba.org/).

## Configuration

Required properties:

| Property                       | Description                          |
| -----------------------------: | ------------------------------------ |
| target.user `<string>`         | Destination user name                |
| target.hosts `<array>`         | Destination hosts list               |
| target.path `<string>`         | Destination path - dir property only |
| dirs `<object>`                | Directories to watch                 |

Optional properties:

| Property                            | Description                                                      |
| ----------------------------------: | ---------------------------------------------------------------- |
| ssh.identityFile `<string>`         | Identity key file path                                           |
| ssh.configFile `<string>`           | Config file path                                                 |
| ssh.port `<integer>`                | Port                                                             |
| ssh.options `<array>`               | Options list                                                     |
| dirs._dir_.includes `<array>`       | Includes list                                                    |
| dirs._dir_.excludes `<array>`       | Excludes list                                                    |
| dirs._dir_.hooks.finished `<array>` | Hooks triggered when job finished without error. It is array of objects in which key is job name and value is predefine name |
| dirs._dir_.hooks.failed `<array>`   | Hooks triggered when job failed. It is array of objects in which key is job name and value is predefine name |
| delay `<integer>`: 100              | Time in ms measured from watcher event, after which job starts   |

Example config:
```json
{
    "target": {
        "user": "user",
        "hosts": [
            "127.0.0.1"
        ]
    },
    "ssh": {
        "identityFile": "/home/user/.ssh/key",
        "port": 22,
        "options": [
            "StrictHostKeyChecking=no"
        ]
    },
    "dirs": {
        "/dir1/": {
            "target": {
                "path": "/dir1-mirror/",
                "user": "root"
            },
            "ssh": {
                "port": 2222
            }
        },
        "/dir2/": {
            "target": {
                "path": "/dir2-mirror/"
            },
            "excludes": [
                "*.git"
            ],
            "hooks": {
                "finished": [
                    {
                        "command": "example"
                    }
                ]
            }
        }
    }
}
``` 
