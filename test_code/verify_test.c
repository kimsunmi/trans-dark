#include "../hedder/polynomial_commit.h"
#include "../hedder/polynomial_open_verify.h"
#include "../hedder/util.h"
int global_num_threads = 1;

// Verify: verify with poly, pp, commit, proof
int main(int argc, char *argv[])
{
	unsigned long long int RunTime_IO = 0;
    int flag = 0;
    _struct_polynomial_pp_ pp = {0};
    _struct_poly_ poly = {0};
    _struct_open_ open = {0};
    _struct_commit_ cm = {0};
    _struct_proof_ proof = {0};

    fmpz_t l;

    fmpz_init(l);
    fmpz_init(cm.C );
    fmpz_init(open.r);
    fmpz_init(open.Q);

	TimerOn();
    Read_poly(&poly);
    Read_pp(&pp);
    Read_Commit("./Txt/commit.txt", &cm);
    Read_proof(&proof);
    RunTime_IO = TimerOff();

    flag = Verify(&pp, &cm, poly.z, poly.fz, &proof);

    printf("Verify_I/O_ %12llu [us]\n", RunTime_IO);
    printf("Verify Result [%d]\n", flag);

	getfilesize("Txt/pp.txt");
	getfilesize("Txt/commit.txt");
	getfilesize("Txt/proof.txt");

    for(int i=0; i<poly.d; i++)
		fmpz_clear(poly.Fx[poly.d-i-1]);
    for(int i =0; i < proof.n; i++) {
        fmpz_clear(proof.s[i]);
        fmpz_clear(proof.y[i]);
        fmpz_clear(proof.D[i]);
        fmpz_clear(pp.R[i]);
    }
    free(poly.Fx);
    free(proof.s);
    free(proof.y);
    free(proof.D);
    free(pp.R);
	return 0;
}
