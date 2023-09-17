#include "../hedder/polynomial_commit.h"
#include "../hedder/polynomial_open_verify.h"
#include "../hedder/util.h"

// group G에서 g, m, 스칼라 q를 뽑아 g*m 연산과 g^q연산의 차이


int main(int argc, char *argv[])
{
    _struct_polynomial_pp_ rsa_pp = {0};
    // struct rsa rsa_pp;
    fmpz_t* pre_table;
    fmpz_t rsa_g;
    fmpz_t tmp_g;

    BIGNUM* tmp = BN_new();
    BIGNUM* bn_4 = BN_new();
	BIGNUM* bn_3 = BN_new();
    
    fmpz_t fmpz_p;
    fmpz_t q;

    int qbit;

    Read_pp(&rsa_pp);

    unsigned long long int Runtime_rsa_g = 0, Runtime_rsa_s = 0;

    fmpz_init(rsa_g);
    fmpz_init(tmp_g);

    pre_table = (fmpz_t*)malloc(sizeof(fmpz_t)*rsa_pp.d);
    for(int i = 0; i < rsa_pp.d; i++){
        fmpz_init(pre_table[i]);
    }

    // generate another g
	BN_generate_prime_ex(tmp, rsa_pp.cm_pp.security_level >> 1,0,NULL,NULL,NULL);
	fmpz_set_str(rsa_g, BN_bn2hex(tmp), 16);

    // 시간차이를 보기 위해 d번 연산 반복-> G*g vs g^q(scalar)
    // G*g
    TimerOn();
    for(int i = 1; i < 1000; i++){
        fmpz_mul(tmp_g, rsa_pp.cm_pp.g, rsa_g);
        fmpz_mod(tmp_g, tmp_g, rsa_pp.cm_pp.G);
    }
    Runtime_rsa_g = TimerOff();

    // g^q
    fmpz_set(pre_table[0], rsa_pp.cm_pp.g);
    qbit = 128*(2*rsa_pp.n + 1)+1;
    fmpz_setbit(q, qbit);

    TimerOn();
    for(int j=1; j < 1000; j++)
    {
        fmpz_powm(tmp_g, rsa_g, q, rsa_pp.cm_pp.G);
    }
    Runtime_rsa_s = TimerOff();

    printf("group_Time %12llu [us]\n", Runtime_rsa_g);
    printf("scalar_Time %12llu [us]\n", Runtime_rsa_s);

    fmpz_clear(fmpz_p);
    fmpz_clear(q);
    for(int i=0; i<rsa_pp.n; i++){
        fmpz_clear(pre_table[i]);
    }
    free(pre_table);

	return 0;
}
