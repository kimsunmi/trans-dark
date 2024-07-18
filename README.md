# Source Code: Efficient Transparent Polynomial Commitments for zk-SNARKs
- This is source code for RSA group.

# Requirments
- gmp, fmp, antic, openssl
## MAC
- Check the library location. Set a soft link to the library on a MAC.

# Start
```
$ make all
$ ./TRANS_Setup ${parameter bit} ${log(degree)}
$ ./TRANS_Commit
$ ./TRANS_Open
$ ./TRANS_Verify 
```
or execute parameter bit in all degree 7~14. The current parameter is 1024, and if you want to modify it, you need to edit the test_script file. 
```
$ make all
$ ./test_script_new
```

- check Unit Operation for 1024/2048/3072. It requires setup paramter.
```
$ ./TRANS_Setup ${parameter bit} ${log(degree)}
$ ./unit_test
```
