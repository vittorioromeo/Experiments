gcc O1

|                  | Bytes | Relative size change |
|------------------|-------|----------------------|
| Baseline         | 257   |   0                  |
| Y-combinator     | 1983  |   +671%              |
| `std::function`  | 4531  |   +1663%             |



gcc O2

|                  | Bytes | Relative size change |
|------------------|-------|----------------------|
| Baseline         | 307   |   0                  |
| Y-combinator     | 1583  |   +415%              |
| `std::function`  | 4572  |   +1389%             |



gcc O3 and ofast

|                  | Bytes | Relative size change |
|------------------|-------|----------------------|
| Baseline         | 307   |   0                  |
| Y-combinator     | 1583  |   +415%              |
| `std::function`  | 4572  |   +1389%             |






clang 01

|                  | Bytes | Relative size change |
|------------------|-------|----------------------|
| Baseline         | 680   |   0                  |
| Y-combinator     | 1583  |   +245%              |
| `std::function`  | 4572  |   +2640%             |



clang 02

|                  | Bytes | Relative size change |
|------------------|-------|----------------------|
| Baseline         | 680   |   0                  |
| Y-combinator     | 1848  |   +171%              |
| `std::function`  | 7146  |   +950%              |



clang 03 and ofast

|                  | Bytes | Relative size change |
|------------------|-------|----------------------|
| Baseline         | 680   |   0                  |
| Y-combinator     | 765   |   +12.5%             |
| `std::function`  | 7146  |   +950%              |