# Command Job

## Description
This job executes given command locally or remotely through ssh.

## Configuration

Required properties:

| Property                       | Description                          |
| -----------------------------: | ------------------------------------ |
| dirs `<object>`                | Directories to watch                 |
| dirs._dir_.exec `<string>`     | Command to be executed               |

Optional properties:

| Property                             | Description                                                      |
| -----------------------------------: | ---------------------------------------------------------------- |
| ssh.identityFile `<string>`          | Identity key file path                                           |
| ssh.configFile `<string>`            | Config file path                                                 |
| ssh.port `<integer>`                 | Port                                                             |
| ssh.options `<array>`                | Options list                                                     |
| ssh.user `<string>`                  | Username                                                         |
| dirs._dir_.remote `<boolean>`: false | If true, command will be executed remotely                       |
| dirs._dir_.fileMask `<string>`       | Start job only if changes was reported in files matching regex. For more information please see [Perl's regex tutorial](http://perldoc.perl.org/perlretut.html) |
| dirs._dir_.hooks.finished `<array>`  | Hooks triggered when job finished without error                  |
| dirs._dir_.hooks.failed `<array>`    | Hooks triggered when job failed                                  |
| delay `<integer>`: 100               | Time in ms measured from watcher event, after which job starts   |
| predefines `<object>`                | Predefines. Can be used as hook                                  |

Example config:
```json
{
    "delay": 100,
    "ssh": {
        "port": 22,
        "hosts": [
            "example.com"
        ],
        "options": [
            "ConnectTimeout=2"
        ],
        "user": "developer"
    },
    "dirs": {
        "/dir1": {
            "remote": true,
            "exec": "mkdir /tmp/test",
            "fileMask": "php$"
        },
        "/dir2/file.txt": {
            "exec": "echo 'file.txt has changed'"
        },
        "/dir3/file.sh": {
            "remote": true,
            "exec": "echo 'file.sh has changed'",
            "ssh": {
                "port": 2222
            }
        }
    },
    "predefines": {
        "example": {
            "exec": "echo 'something happened'"
        }
    }
}
``` 
