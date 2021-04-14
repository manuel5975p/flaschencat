# flaschencat
A very fast cat implementation, even faster than [fcat](https://github.com/mre/fcat "A slower fastcat")  
Here a little benchmark of how fast it can pipe a file to /dev/null:  
*program* file.txt > /dev/null with *program* ∈ {cat, fcat, flaschencat}  

|         | 1 GB File           | 5 GB File  |
| ------------- |:-------------:| -----:|
| cat      |    79.6 ms | 468.2 ms|
| fcat      |         57.1 ms  | 265.2 ms|
| flaschencat |          **38.6 ms**  |**195.4 ms** |

Here another benchmark of how fast it can pipe a file through itself and then to /dev/null:  
*program* file.txt | *program* > /dev/null with *program* ∈ {cat, fcat, flaschencat}  

|         | 1GB File           | 5 GB File  |
| ------------- |:-------------:| -----:|
| cat      |    305.1 ms |1.559 s |
| fcat      |         74.8 ms  |324.5 ms |
| flaschencat |         **57.0  ms**  |**199.3 ms** |
