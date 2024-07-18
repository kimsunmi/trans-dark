#include "../hedder/polynomial_commit.h"
#include "../hedder/polynomial_open_verify.h"
#include "../hedder/util.h"
#include <openssl/rand.h>

// group G에서 g, m, 스칼라 q를 뽑아 g*m 연산과 g^q연산의 차이


int main(int argc, char *argv[])
{
    unsigned long long int Runtime_rsa_g = 0,Runtime_rsa_g2 = 0, Runtime_rsa_s = 0, Runtime_rsa_s2=0;

    _struct_polynomial_pp_ rsa_pp = {0};
    size_t count;
    unsigned char *bin;
    mpz_t mpz_q;
    
    // struct rsa rsa_pp;
    fmpz_t rsa_g;
    fmpz_t rsa_g2;
    fmpz_t tmp_g;
    fmpz_t tmp2_g;
    fmpz_t q;
    fmpz_t alpha;

    int qbit;

    // Read_pp(&rsa_pp);
    rsa_pp.n = 10;
    rsa_pp.d = 1<<rsa_pp.n;
    rsa_pp.cm_pp.security_level = atoi(argv[1]);
    
    fmpz_init(rsa_g);
    fmpz_init(rsa_g2);
    fmpz_init(tmp_g);
    fmpz_init(tmp2_g);
    fmpz_init(alpha);

	BIGNUM* p = BN_new();
	BIGNUM* rsa_q = BN_new();
	BIGNUM* tmp = BN_new();
	BIGNUM* tmp2 = BN_new();
	BN_CTX* ctx = BN_CTX_new();
    
    printf("param, d: %d %d\n", rsa_pp.cm_pp.security_level, rsa_pp.d);

	do{
		BN_generate_prime_ex(p,(rsa_pp.cm_pp.security_level>>1),0,NULL,NULL,NULL);
		BN_generate_prime_ex(rsa_q,(rsa_pp.cm_pp.security_level>>1),0,NULL,NULL,NULL);
		BN_mul(tmp,p,rsa_q, ctx);
		// G = lamda size prime*prime
		fmpz_set_str(rsa_pp.cm_pp.G, BN_bn2hex(tmp), 16);
	}while(BN_num_bits(tmp) != rsa_pp.cm_pp.security_level);

    // 시간차이를 보기 위해 d번 연산 반복-> G*g vs g^q(scalar)
    // g^q
    qbit = 128*(2*rsa_pp.n + 1)+1;
    fmpz_setbit(q, qbit);
    // G*g
    // g^q
    for(int j=0; j<1000;j++){
        // g = lamda/2 size random prime 
        BN_generate_prime_ex(tmp,rsa_pp.cm_pp.security_level >> 1,0,NULL,NULL,NULL);
        fmpz_set_str(rsa_g, BN_bn2hex(tmp), 16);

        // generate another g
        BN_generate_prime_ex(tmp, rsa_pp.cm_pp.security_level >> 1,0,NULL,NULL,NULL);
        fmpz_set_str(rsa_g2, BN_bn2hex(tmp), 16);
        
        // TimerOn();
        // fmpz_mul(tmp_g, rsa_g, rsa_g2);
        // Runtime_rsa_g += TimerOff();

        if (RAND_poll() != 1) {
            printf("RAND_poll() failed\n");
        }
        fmpz_set(tmp2_g, rsa_g2);
        fmpz_set(tmp_g, rsa_g);
        // for(int k=0;k<10;k++){
            // BN_rand(tmp, 32, 0, 0);
            // fmpz_set_str(alpha, BN_bn2hex(tmp), 16); 
            // TimerOn();
            // fmpz_powm(tmp2_g, rsa_g, alpha, rsa_pp.cm_pp.G);
            // Runtime_rsa_g+= TimerOff();

            BN_rand(tmp, 256, 0, 0);
            fmpz_set_str(alpha, BN_bn2hex(tmp), 16); 
            TimerOn();
            fmpz_powm(tmp2_g, rsa_g, alpha, rsa_pp.cm_pp.G);
            Runtime_rsa_g2+= TimerOff();
            
            // BN_rand(tmp, 2688, 0, 0);
            // fmpz_set_str(alpha, BN_bn2hex(tmp), 16); 
            // TimerOn();
            // fmpz_powm(tmp2_g, rsa_g, alpha, rsa_pp.cm_pp.G);
            // Runtime_rsa_s+= TimerOff();
            // TimerOn();
            // fmpz_mul(tmp_g, tmp_g, alpha);
            // Runtime_rsa_g += TimerOff();
        // }

    }
    // fmpz_set(tmp_g, rsa_g);

    // TimerOn();
    // fmpz_powm(tmp_g, tmp_g, q, rsa_pp.cm_pp.G);
    // fmpz_mod(tmp_g, tmp_g, rsa_pp.cm_pp.G);
    // Runtime_rsa_s = TimerOff();
    // fmpz_set(tmp_g, rsa_g);

    // TimerOn();
    // for(int j=0; j < rsa_pp.d; j++)
    // {
    //     fmpz_powm(tmp_g, tmp_g, q, rsa_pp.cm_pp.G);
    //     fmpz_mod(tmp_g, tmp_g, rsa_pp.cm_pp.G);
    // }
    // Runtime_rsa_s2 = TimerOff();

    // for(int j=0;j<1000;j++){
    //     fmpz_set(tmp_g, rsa_g);
    //     if (RAND_poll() != 1) {
    //         printf("RAND_poll() failed\n");
    //     }

    //     BN_rand(tmp, 2688, 0, 0);
    //     fmpz_set_str(alpha, BN_bn2hex(tmp), 16); 
        
    //     TimerOn();
    //     fmpz_powm(tmp_g, tmp_g, alpha, rsa_pp.cm_pp.G);
    //     Runtime_rsa_g2+= TimerOff();
    // }

    printf("g1^a_32 %12llu [us]\n", Runtime_rsa_g);
    printf("g1^b_256 %12llu [us]\n", Runtime_rsa_g2);
    printf("g^c_q-1 %12llu [us]\n", Runtime_rsa_s);
    //printf("g^q^d_exp_Time %12llu [us]\n", Runtime_rsa_s2);

    fmpz_clear(rsa_g);
    fmpz_clear(rsa_g2);
    fmpz_clear(tmp_g);
    fmpz_clear(tmp2_g);
    fmpz_clear(alpha);
    fmpz_clear(q);
	return 0;
}
