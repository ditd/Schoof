#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<flint/flint.h>
#include<flint/fmpz.h>
#include<flint/fq.h>
#include<gmp.h>
#include<flint/fq_poly.h>

void fnext_prime(fmpz_t l){
	fmpz_add_ui(l, l, 2); // res = l+2
	while(!fmpz_is_prime(l)){ // 0 ou -1
		fmpz_add_ui(l,l,2); // res = res+2
	}
}

void Psi(fq_t a,fq_t b,fmpz_t l,fq_poly_t Psi1,fq_ctx_t ctx){
	fmpz_t n,i,k,tmp4;
	fmpz_init(i);fmpz_zero(i);
	fmpz_init(k);fmpz_set_si(k,5);
	if(fmpz_cmp(k,l)<=0){fmpz_add_ui(k,l,1);}
	fq_poly_t Psi[fmpz_get_ui(k)];
	fq_t tmp,tmp1;
	fq_poly_t tmp2,tmp3,tmp5,y2;
	
	//Initialisation des variables
	while(fmpz_cmp(i,k)<=0){
 		fq_poly_init(Psi[fmpz_get_ui(i)], ctx);
 		fmpz_add_ui(i,i,1);
	}
	fq_init(tmp,ctx);fq_init(tmp1,ctx);fq_poly_init(tmp2,ctx);
	fq_poly_init(tmp3,ctx);fq_poly_init(tmp5,ctx);
	fq_poly_init(y2,ctx);fmpz_init(n);
	
	//Poly y2
	fq_set_si(tmp1, 1, ctx);
	fq_poly_set_coeff(y2, 3, tmp1, ctx);//x^3
	fq_poly_set_coeff(y2, 1, a, ctx);//a*x
	fq_poly_set_coeff(y2, 0, b, ctx);//b
		
	//Psi_0 = 0
	fq_poly_zero(Psi[0], ctx); 
	
	//Psi_1 = 1
	fq_poly_one(Psi[1], ctx); 
	
	//Psi_2 = 2 
	fq_set_si(tmp, 2, ctx);
	fq_poly_set_fq(Psi[2], tmp, ctx);
	
	//Psi_3 = 3x^4+6ax^2+12bx-a^2
	fq_set_si(tmp, 3, ctx);fq_poly_set_coeff(Psi[3], 4, tmp, ctx);//3*x^4
	fq_mul_ui(tmp, a, 6, ctx);fq_poly_set_coeff(Psi[3], 2, tmp, ctx);//6*a*x^2
	fq_mul_ui(tmp, b, 12, ctx);fq_poly_set_coeff(Psi[3], 1, tmp, ctx);//12*b*x
	fq_sqr(tmp, a, ctx);fq_neg(tmp, tmp, ctx);
	fq_poly_set_coeff(Psi[3], 0, tmp, ctx);//-a^2
	
	//Psi_4 = 4(x^6+5ax^4+20bx^3-5a^2x^2-4abx-8b^2-a^3)
	fq_mul_ui(tmp, tmp,5, ctx);
	fq_poly_set_coeff(Psi[4], 2, tmp, ctx);//-5a^2x^2
	fq_set_si(tmp,1,ctx);fq_poly_set_coeff(Psi[4], 6, tmp, ctx);//x^6
	fq_mul_ui(tmp,a,5,ctx);fq_poly_set_coeff(Psi[4], 4, tmp, ctx);//5ax^4
	fq_mul_ui(tmp, b, 20, ctx);fq_poly_set_coeff(Psi[4], 3, tmp, ctx);//20bx^3
	fq_neg(tmp,a,ctx);fq_mul_ui(tmp, tmp, 4, ctx);fq_mul(tmp, tmp, b, ctx);
	fq_poly_set_coeff(Psi[4], 1, tmp, ctx);//-4abx
	fq_sqr(tmp,b,ctx);fq_neg(tmp,tmp,ctx);fq_mul_ui(tmp,tmp,8,ctx);fq_pow_ui(tmp1,a,3,ctx);
	fq_sub(tmp,tmp,tmp1,ctx);fq_poly_set_coeff(Psi[4], 0, tmp, ctx);//-8b^2-a^3
	fq_set_si(tmp,4,ctx);fq_poly_scalar_mul_fq(Psi[4],Psi[4],tmp,ctx);
	
	/* Psi5 = Psi4*(Psi2^3)-(Psi3^3)*Psi1 */
	fq_poly_pow(tmp2,Psi[2],3,ctx);fq_poly_mul(tmp2,tmp2,Psi[4],ctx);
	fq_poly_sqr(tmp5,y2,ctx);fq_poly_mul(tmp2,tmp2,tmp5,ctx);
	fq_poly_pow(tmp3,Psi[3],3,ctx);fq_poly_sub(Psi[5],tmp2,tmp3,ctx);
	
	fmpz_fdiv_q_ui(n,l,2);
	
	/* Psim, si n>=3 */
	if(fmpz_cmp_ui(n,3)>=0){
		fmpz_set_si(i,3);
		while(fmpz_cmp(i,n)<=0){
			fmpz_sub_ui(tmp4,i,1);fq_poly_sqr(tmp2,Psi[fmpz_get_ui(tmp4)],ctx);
			fmpz_add_ui(tmp4,i,2);fq_poly_mul(tmp2,tmp2,Psi[fmpz_get_ui(tmp4)],ctx);
			fmpz_add_ui(tmp4,i,1);fq_poly_sqr(tmp3,Psi[fmpz_get_ui(tmp4)],ctx);
			fmpz_sub_ui(tmp4,i,2);fq_poly_mul(tmp3,tmp3,Psi[fmpz_get_ui(tmp4)],ctx);
			fq_poly_sub(tmp2,tmp2,tmp3,ctx);
			fq_poly_mul(tmp2,tmp2,Psi[fmpz_get_ui(i)],ctx);
			fq_set_si(tmp,2,ctx);fq_inv(tmp,tmp,ctx);
			fmpz_mul_ui(tmp4,i,2);
			fq_poly_scalar_mul_fq(Psi[fmpz_get_ui(tmp4)],tmp2,tmp,ctx);
					
			if(fmpz_is_odd(i)){
				fq_poly_pow(tmp2,Psi[fmpz_get_ui(i)],3,ctx);
				fmpz_add_ui(tmp4,i,2);fq_poly_mul(tmp2,tmp2,Psi[fmpz_get_ui(tmp4)],ctx);
				fmpz_add_ui(tmp4,i,1);fq_poly_pow(tmp3,Psi[fmpz_get_ui(tmp4)],3,ctx);
				fmpz_sub_ui(tmp4,i,1);fq_poly_mul(tmp3,tmp3,Psi[fmpz_get_ui(tmp4)],ctx);
				fq_poly_sqr(tmp5,y2,ctx);fq_poly_mul(tmp3,tmp3,tmp5,ctx);
				fmpz_mul_ui(tmp4,i,2);fmpz_add_ui(tmp4,tmp4,1);
				fq_poly_sub(Psi[fmpz_get_ui(tmp4)],tmp2,tmp3,ctx);
			}
			else{
				fq_poly_pow(tmp2,Psi[fmpz_get_ui(i)],3,ctx);
				fmpz_add_ui(tmp4,i,2);fq_poly_mul(tmp2,tmp2,Psi[fmpz_get_ui(tmp4)],ctx);
				fq_poly_sqr(tmp5,y2,ctx);fq_poly_mul(tmp2,tmp2,tmp5,ctx);
				fmpz_add_ui(tmp4,i,1);fq_poly_pow(tmp3,Psi[fmpz_get_ui(tmp4)],3,ctx);
				fmpz_sub_ui(tmp4,i,1);fq_poly_mul(tmp3,tmp3,Psi[fmpz_get_ui(tmp4)],ctx);
				fmpz_mul_ui(tmp4,i,2);fmpz_add_ui(tmp4,tmp4,1);
				fq_poly_sub(Psi[fmpz_get_ui(tmp4)],tmp2,tmp3,ctx);
				
			}
			fmpz_add_ui(i,i,1);
		}
	}
		
	fq_poly_set(Psi1,Psi[fmpz_get_ui(l)],ctx);
	
	fmpz_zero(i);
	while(fmpz_cmp(i,k)<=0){
 		fq_poly_clear(Psi[fmpz_get_ui(i)], ctx);
 		fmpz_add_ui(i,i,1);
	}
	fmpz_clear(n);fmpz_clear(i);fmpz_clear(k);fmpz_clear(tmp4);
	fq_clear(tmp,ctx);fq_clear(tmp1,ctx);fq_poly_clear(tmp2,ctx);
	fq_poly_clear(tmp3,ctx);fq_poly_clear(tmp5,ctx);
	fq_poly_clear(y2,ctx);
}

void schoof(fq_t a,fq_t b,fq_ctx_t ctx,fmpz_t q){
	ulong e;
	fq_t tmp1;fq_init(tmp1,ctx);
	fmpz_t l, c, q_sqrt,tmp,tmp2,tmp3,t,p,j,q_bar,w;
	fmpz_init(l);fmpz_init(c);fmpz_init(q_sqrt);fmpz_init(tmp);
	fmpz_init(tmp2);fmpz_init(tmp3);fmpz_init(t);fmpz_init(p);
	fmpz_init(j);fmpz_init(q_bar);fmpz_init(w);
	fq_poly_t y2,Phi1,Phi2,Phi3,Phi4,poly,poly1,poly2,Psi1,Psi2,beta,alpha;
	fq_poly_t teta1,gamma1,teta2,gamma2,absc,ordo;
	fq_poly_init(y2,ctx);fq_poly_init(Phi1,ctx);
	fq_poly_init(Phi2,ctx);fq_poly_init(Phi3,ctx);fq_poly_init(Phi4,ctx);
	fq_poly_init(poly,ctx);fq_poly_init(poly1,ctx);
	fq_poly_init(poly2,ctx);fq_poly_init(Psi1,ctx);
	fq_poly_init(Psi2,ctx);fq_poly_init(beta,ctx);
	fq_poly_init(alpha,ctx);fq_poly_init(Phi3,ctx);
	fq_poly_init(gamma1,ctx);fq_poly_init(teta1,ctx);
	fq_poly_init(gamma2,ctx);fq_poly_init(teta2,ctx);
	fq_poly_init(absc,ctx);fq_poly_init(ordo,ctx);
	
	fmpz_sqrt(q_sqrt, q);fmpz_mul_ui(q_sqrt, q_sqrt, 4);// borne 4*sqrt(p)
	
	//Construction du poly de l'end de frobenius Phi1 = x^q - x
	fq_set_si(tmp1, 1, ctx);
	fq_poly_set_coeff(Phi1, fmpz_get_ui(q), tmp1, ctx);
	fq_neg(tmp1, tmp1, ctx);
	fq_poly_set_coeff(Phi1, 1, tmp1, ctx);
	
	//Construction de Phi2 = x^q² - x
	fq_set_si(tmp1, 1, ctx);
	fq_poly_set_coeff(Phi2, fmpz_get_ui(q)*fmpz_get_ui(q), tmp1, ctx);
	fq_neg(tmp1, tmp1, ctx);
	fq_poly_set_coeff(Phi2, 1, tmp1, ctx);
	
	//Construction de X^q² + X^q + X = Phi3
	fq_set_si(tmp1, 1, ctx);
	fq_poly_set_coeff(Phi3, fmpz_get_ui(q)*fmpz_get_ui(q), tmp1, ctx);
	fq_poly_set_coeff(Phi3, fmpz_get_ui(q), tmp1, ctx);
	fq_poly_set_coeff(Phi3, 1, tmp1, ctx);
	
	//Poly y2
	fq_set_si(tmp1, 1, ctx);
	fq_poly_set_coeff(y2, 3, tmp1, ctx);//x^3
	fq_poly_set_coeff(y2, 1, a, ctx);//a*x
	fq_poly_set_coeff(y2, 0, b, ctx);//b
	
	//pgcd(Phi1, y2)
	fq_poly_gcd(poly, Phi1, y2, ctx);
	
	fmpz_set_si(l,2);
	
	//Pour l=2
	if(fq_poly_is_one(poly,ctx)){
		fmpz_set_si(t, 1);
	}
	else{fmpz_set_si(t, 0);}
	fmpz_print(t);printf("mod");fmpz_print(l);flint_printf("\n");
	
	fmpz_print(t);fmpz_print(l);flint_printf("\n");
	
	fmpz_sqrt(q_sqrt,q);fmpz_mul_ui(q_sqrt,q_sqrt,4);
	fmpz_set_si(l,3);fmpz_set_si(c,2);
	
	while(fmpz_cmp(c,q_sqrt)<=0){
		//q_bar = q mod l
		fmpz_mod(q_bar, q, l);
		/*fmpz_sub_ui(tmp,l,1);
		fmpz_divexact_ui(tmp, tmp, 2); //tmp = l-1/2
		if(fmpz_cmp(q_bar, tmp)>0){
			fmpz_sub(q_bar, q_bar, l);//q_bar = q_bar - l
		}*/
		
		//Cas q_bar impair
	    	if(fmpz_is_odd(q_bar)){
			Psi(a,b,q_bar,Psi1,ctx);fq_poly_sqr(poly, Psi1, ctx);
		    	fq_poly_mul(poly, poly, Phi2, ctx);
		    	
		    	fmpz_sub_ui(tmp,q_bar,1);
			Psi(a,b,tmp,Psi1,ctx);fmpz_add_ui(tmp,q_bar,1);
			Psi(a,b,tmp,Psi2,ctx);
		    	fq_poly_mul(poly1, Psi1, Psi2, ctx);
		    	fq_poly_mul(poly1, poly1, y2, ctx);
		    	fq_poly_add(poly, poly, poly1, ctx);
	   	}

	    	//Cas q_bar pair
	    	else{
		    	Psi(a,b,q_bar,Psi1,ctx);fq_poly_sqr(poly, Psi1, ctx);
		    	fq_poly_mul(poly, poly, Phi2, ctx);
		    	fq_poly_mul(poly, poly, y2, ctx);
		    	
			fmpz_sub_ui(tmp,q_bar,1);
			Psi(a,b,tmp,Psi1,ctx);fmpz_add_ui(tmp,q_bar,1);
			Psi(a,b,tmp,Psi2,ctx);
		    	fq_poly_mul(poly1, Psi1, Psi2, ctx);
		    	
		   	fq_poly_add(poly, poly, poly1, ctx);
		}
		
		Psi(a,b,l,Psi1,ctx);
		fq_poly_gcd(poly,poly,Psi1,ctx);
		
		char* var="x";
		fq_poly_print_pretty(poly,var,ctx);printf("l=");fmpz_print(l);flint_printf("\n");
		
		//Il existe un point de l-torsion P tq Phi^2(P)=+-[q_bar]P
		if(!fq_poly_is_one(poly,ctx)){
			//Cas 1: Phi^2(P)=-[q_bar]P
			printf("jac=");printf("%d",fmpz_jacobi(q, l));printf("l=");fmpz_print(l);flint_printf("\n");
			if(fmpz_jacobi(q, l) == -1){//q n'est pas un résidu quadratique
				fmpz_zero(tmp); // tmp = 0
				fmpz_CRT(t, t, c, tmp, l, 1); // TRC: t=0 mais on fait le calcul
				fmpz_print(tmp);printf("mod");fmpz_print(l);flint_printf("\n");
			}

			//Cas 2: Phi^2(P)=[q_bar]P
			else{
				fmpz_sqrtmod(w, q, l);
				printf("sqrt=");fmpz_print(w);printf("l=");fmpz_print(l);flint_printf("\n");
				//Cas w impair
				if(fmpz_is_odd(w)){
					Psi(a,b,w,Psi1,ctx);
			    		fq_poly_sqr(poly, Psi1, ctx);
			    		fq_poly_mul(poly, poly, Phi1, ctx);
			    		
					fmpz_sub_ui(tmp,w,1);
					Psi(a,b,tmp,Psi1,ctx);fmpz_add_ui(tmp,w,1);
					Psi(a,b,tmp,Psi2,ctx);
			    		fq_poly_mul(poly1, Psi1, Psi2, ctx);
			    		fq_poly_mul(poly1, poly1, y2, ctx);
			    		
			    		fq_poly_add(poly, poly, poly1, ctx);
			    	}

			    	//Cas w pair
			    	else{
			    		Psi(a,b,w,Psi1,ctx);fq_poly_sqr(poly, Psi1, ctx);
			    		fq_poly_mul(poly, poly, Phi1, ctx);
			    		fq_poly_mul(poly, poly, y2, ctx);
			    		
					fmpz_sub_ui(tmp,w,1);
					Psi(a,b,tmp,Psi1,ctx);fmpz_add_ui(tmp,w,1);
					Psi(a,b,tmp,Psi2,ctx);
			    		fq_poly_mul(poly1, Psi1, Psi2, ctx);
			    		
			    		fq_poly_add(poly, poly, poly1, ctx);
			    	}

			    	//Calcul du pgcd
				Psi(a,b,l,Psi1,ctx);
				fq_poly_gcd(poly, poly, Psi1, ctx);
				
				/*char* var="x";
				fq_poly_print_pretty(poly,var,ctx);printf("l=");fmpz_print(l);flint_printf("\n");*/
				
				//Cas pgcd=1 cad w et -w ne sont pas solution: t=0[l]
				if(fq_poly_is_one(poly, ctx)){
					fmpz_zero(tmp); // tmp = 0
					fmpz_CRT(t, t, c, tmp, l, 1);
					fmpz_print(tmp);printf("mod");fmpz_print(l);flint_printf("\n");
				}
				else{//Cas pgcd différent de 1
					//Cas w pair
					if(fmpz_is_even(w)){
				    		fmpz_add_ui(tmp, q, 3);
			    			fmpz_divexact_ui(tmp, tmp, 2);
			    		}
			    		//Cas w impair
			    		else{
			    			fmpz_sub_ui(tmp, q, 1);
			    			fmpz_divexact_ui(tmp, tmp, 2);
			    		}
			    		e=fmpz_get_ui(tmp);
			    		fq_poly_pow(poly, y2, e, ctx);
					Psi(a,b,w,Psi1,ctx);
			    		fq_poly_pow(poly1, Psi1, 3, ctx);
			    		fq_poly_mul(poly, poly, poly1, ctx);
					fq_set_si(tmp1,4,ctx);
			    		fq_poly_scalar_mul_fq(poly, poly, tmp1, ctx);
			    		
			    		fmpz_sub_ui(tmp,w,1);Psi(a,b,tmp,Psi1,ctx);
			    		fq_poly_pow(poly1, Psi1, 2, ctx);
					fmpz_add_ui(tmp,w,2);Psi(a,b,tmp,Psi1,ctx);
			    		fq_poly_mul(poly1, poly1, Psi1, ctx);
			    			
			    		fq_poly_sub(poly, poly, poly1, ctx);
			    		
					fmpz_add_ui(tmp,w,1);Psi(a,b,tmp,Psi1,ctx);
					fq_poly_pow(poly1, Psi1, 2, ctx);
					fmpz_sub_ui(tmp,w,2);
					if(fmpz_cmp_ui(tmp,0)<0){
						fmpz_neg(tmp,tmp);
						Psi(a,b,tmp,Psi1,ctx);
						fq_poly_neg(Psi1,Psi1,ctx);
					}
					else{Psi(a,b,tmp,Psi1,ctx);}
			    		fq_poly_mul(poly1, poly1, Psi1, ctx);
			    		
			    		fq_poly_add(poly, poly, poly1, ctx);
			    				
			    		//Calcul du pgcd
					Psi(a,b,l,Psi1,ctx);
				    	fq_poly_gcd(poly, poly, Psi1, ctx);
				    		
			    		fmpz_mul_si(tmp, w, 2);//tmp=2*w

			    		//pgcd = 1 cad t n'est pas 2w[l]: t = -2w[l]
			    		if(fq_poly_is_one(poly, ctx)){
				    		fmpz_neg(tmp, tmp);
				    	}

			      		fmpz_CRT(t, t, c, tmp, l, 1);//TRC
			      		fmpz_print(tmp);printf("mod");fmpz_print(l);flint_printf("\n");
				
				}
			}

		}
		else{	//Il n'existe pas point de l-torsion tq Phi^2(P)=+-[q_bar]P
			fmpz_one(j);
			fmpz_sub_ui(tmp3,l,1);fmpz_divexact_ui(tmp3,tmp3,2);
			while(fmpz_cmp(j,tmp3)<=0){
				if(fmpz_is_even(q_bar)){
					printf("j=");fmpz_print(j);printf("l=");fmpz_print(l);flint_printf("\n");
					//Calcul de alpha
					fmpz_sub_ui(tmp,q_bar,1);Psi(a,b,tmp,Psi1,ctx);
					fq_poly_sqr(poly, Psi1, ctx);
					fmpz_add_ui(tmp,q_bar,2);Psi(a,b,tmp,Psi1,ctx);
					fq_poly_mul(poly, poly, Psi1, ctx);
					fmpz_add_ui(tmp,q_bar,1);Psi(a,b,tmp,Psi1,ctx);
					fq_poly_sqr(poly1, Psi1, ctx);
					fmpz_sub_ui(tmp,q_bar,2);Psi(a,b,tmp,Psi1,ctx);
					fq_poly_mul(poly1, poly1, Psi1, ctx);

					fq_poly_sub(poly, poly, poly1, ctx);

					Psi(a,b,q_bar,Psi1,ctx);
					fq_poly_pow(poly1, Psi1, 3, ctx);
 
 					fmpz_pow_ui(tmp, q, 2);
					fmpz_add_ui(tmp, tmp, 3);
					fmpz_divexact_ui(tmp, tmp, 2);

					fq_poly_pow(alpha, y2, fmpz_get_ui(tmp), ctx);
					fq_poly_mul(alpha, alpha, poly1, ctx);
					fq_set_si(tmp1, 4, ctx);
					fq_poly_scalar_mul_fq(alpha, alpha, tmp1, ctx);

					fq_poly_sub(alpha, poly, alpha, ctx);

					//Calcul de beta
					fmpz_sub_ui(tmp,q_bar,1);Psi(a,b,tmp,Psi1,ctx);
					fmpz_add_ui(tmp,q_bar,1);Psi(a,b,tmp,Psi2,ctx);
					fq_poly_mul(poly, Psi1, Psi2, ctx);
					
					Psi(a,b,q_bar,Psi1,ctx);
					fq_poly_sqr(poly1, Psi1, ctx);
					fq_poly_mul(poly1,poly1,Phi2,ctx);
					fq_poly_mul(poly1, poly1, y2, ctx);
					
					fq_poly_add(poly, poly1, poly, ctx);

					Psi(a,b,q_bar,Psi1,ctx);
					fq_poly_mul(poly, poly, Psi1, ctx);
					fq_poly_mul(poly, poly, y2, ctx);
					fq_set_si(tmp1, 4, ctx);
					fq_poly_scalar_mul_fq(poly, poly, tmp1, ctx);
					fq_poly_neg(beta,poly,ctx);//beta
						
					//Calcul de gamma1 et teta1
					fmpz_sub_ui(tmp,q_bar,1);Psi(a,b,tmp,Psi1,ctx);
					fmpz_add_ui(tmp,q_bar,1);Psi(a,b,tmp,Psi2,ctx);
					fq_poly_mul(poly,Psi1,Psi2,ctx);
					
					Psi(a,b,q_bar,Psi1,ctx);fq_poly_sqr(poly1,Psi1,ctx);
					fq_poly_mul(poly1,poly1,Phi3,ctx);
					fq_poly_mul(poly1,poly1,y2,ctx);
					
					fq_poly_sub(poly,poly,poly1,ctx);
					
					fq_poly_sqr(poly1,beta,ctx);
					fq_poly_mul(poly,poly,poly1,ctx);
					
					Psi(a,b,q_bar,Psi1,ctx);fq_poly_sqr(poly1,Psi1,ctx);
					fq_poly_mul(poly1,poly1,y2,ctx);
					fq_poly_mul(poly1,poly1,y2,ctx);
					fq_poly_sqr(poly2,alpha,ctx);
					fq_poly_mul(poly1,poly1,poly2,ctx);
					
					fq_poly_add(gamma1,poly,poly1,ctx);//gamma1

					Psi(a,b,q_bar,Psi1,ctx);
					fq_poly_sqr(poly,Psi1,ctx);
					fq_poly_sqr(poly1,beta,ctx);
					fq_poly_mul(poly1,poly1,y2,ctx);
					fq_poly_mul(teta1,poly,poly1,ctx);//teta1			
						
					//Calcul de gamma2 et teta2
					fq_poly_pow(poly,beta,3,ctx);fq_poly_mul(poly,poly,y2,ctx);
					Psi(a,b,q_bar,Psi1,ctx);fq_poly_sqr(poly1,Psi1,ctx);
					fq_poly_mul(teta2,poly,poly1,ctx);//teta2
					
					fq_set_si(tmp1,2,ctx);fmpz_mul(tmp,q,q);e=fmpz_get_ui(tmp);
					fq_poly_set_coeff(Phi4,e,tmp1,ctx);
					fq_set_si(tmp1,1,ctx);fq_poly_set_coeff(Phi4,1,tmp1,ctx);
						
					Psi(a,b,q_bar,Psi1,ctx);
					fq_poly_sqr(poly,Psi1,ctx);fq_poly_mul(poly,Phi4,poly,ctx);
					fq_poly_mul(poly,poly,alpha,ctx);fq_poly_sqr(poly1,beta,ctx);
					fq_poly_mul(poly1,poly1,y2,ctx);fq_poly_mul(poly,poly,poly1,ctx);
						
					fq_poly_pow(poly1,alpha,3,ctx);Psi(a,b,q_bar,Psi1,ctx);
					fq_poly_sqr(poly2,Psi1,ctx);fq_poly_mul(poly1,poly1,poly2,ctx);
					fq_poly_mul(poly1,poly1,y2,ctx);fq_poly_mul(poly1,poly1,y2,ctx);
						
					fq_poly_sub(poly,poly,poly1,ctx);
											
					fmpz_sub_ui(tmp,q_bar,1);Psi(a,b,tmp,Psi1,ctx);
					fmpz_add_ui(tmp,q_bar,1);Psi(a,b,tmp,Psi2,ctx);
					fq_poly_mul(poly1,Psi1,Psi2,ctx);fq_poly_mul(poly1,poly1,alpha,ctx);
					fq_poly_sqr(poly2,beta,ctx);
					fq_poly_mul(poly1,poly1,poly2,ctx);
						
					fq_poly_sub(poly,poly,poly1,ctx);
						
					fmpz_mul(tmp,q,q);fmpz_add_ui(tmp,tmp,1);
					fmpz_divexact_ui(tmp,tmp,2);e=fmpz_get_ui(tmp);
					fq_poly_pow(poly1,y2,e,ctx);Psi(a,b,q_bar,Psi1,ctx);
					fq_poly_sqr(poly2,Psi1,ctx);fq_poly_mul(poly1,poly1,poly2,ctx);
					fq_poly_pow(poly2,beta,3,ctx);fq_poly_mul(poly1,poly1,poly2,ctx);
						
					fq_poly_sub(poly,poly,poly1,ctx);
					
					fmpz_add_ui(tmp, q, 1);fmpz_divexact_ui(tmp, tmp, 2);
					e=fmpz_get_ui(tmp);fq_poly_pow(poly1,y2,e,ctx);
					fq_set_si(tmp1,4,ctx);
					fq_poly_scalar_mul_fq(poly1,poly1,tmp1,ctx);
						
					fq_poly_mul(gamma2,poly,poly1,ctx);//gamma2
						
					if(fmpz_is_even(j)){
						//Verification abcisse
						fmpz_mul_ui(tmp,q,2);e=fmpz_get_ui(tmp);
						Psi(a,b,j,Psi1,ctx);fq_poly_pow(poly,Psi1,e,ctx);
						e=fmpz_get_ui(q);
						fq_poly_pow(poly1,y2,e,ctx);
						fq_poly_mul(poly,poly,poly1,ctx);
						fq_poly_mul(poly,poly,gamma1,ctx);
						
						e=fmpz_get_ui(q);fmpz_sub_ui(tmp,j,1);
						Psi(a,b,tmp,Psi1,ctx);fq_poly_pow(poly1,Psi1,e,ctx);
						fmpz_add_ui(tmp,j,1);Psi(a,b,tmp,Psi1,ctx);
						fq_poly_pow(poly2,Psi1,e,ctx);
						fq_poly_mul(poly1,poly1,poly2,ctx);
						fq_poly_mul(poly1,poly1,teta1,ctx);
						fq_poly_add(absc,poly,poly1,ctx);
						
						//Verification ordonnée
						fmpz_mul_ui(tmp,q,3);e=fmpz_get_ui(tmp);
						Psi(a,b,j,Psi1,ctx);fq_poly_pow(poly,Psi1,e,ctx);
						fmpz_mul_ui(tmp,q,3);fmpz_sub_ui(tmp,tmp,1);
						fmpz_divexact_ui(tmp,tmp,2);e=fmpz_get_ui(tmp);
						fq_poly_pow(poly1,y2,e,ctx);
						fq_poly_mul(poly,poly,poly1,ctx);
						fq_poly_mul(poly,poly,gamma2,ctx);
						
						fmpz_sub_ui(tmp,j,1);Psi(a,b,tmp,Psi1,ctx);
						fq_poly_sqr(poly1,Psi1,ctx);
						fmpz_add_ui(tmp,j,2);Psi(a,b,tmp,Psi1,ctx);
						fq_poly_mul(poly1,poly1,Psi1,ctx);
						fmpz_add_ui(tmp,j,1);Psi(a,b,tmp,Psi1,ctx);
						fq_poly_sqr(poly2,Psi1,ctx);
						fmpz_sub_ui(tmp,j,2);
						if(fmpz_cmp_ui(tmp,0)<0){
							fmpz_neg(tmp,tmp);
							Psi(a,b,tmp,Psi1,ctx);
							fq_poly_neg(Psi1,Psi1,ctx);
						}
						else{Psi(a,b,tmp,Psi1,ctx);}
						/*char* var="x";
						fq_poly_print_pretty(Psi1,var,ctx);flint_printf("\n");*/
						fq_poly_mul(poly2,poly2,Psi1,ctx);
						fq_poly_sub(poly1,poly1,poly2,ctx);e=fmpz_get_ui(q);
						fq_poly_pow(poly1,poly1,e,ctx);fq_poly_mul(poly1,poly1,teta2,ctx);
						fmpz_sub_ui(tmp,q,1);fmpz_divexact_ui(tmp,tmp,2);
						e=fmpz_get_ui(tmp);fq_poly_pow(poly2,y2,e,ctx);
						fq_poly_mul(poly1,poly1,poly2,ctx);
						
						fq_poly_sub(ordo,poly,poly1,ctx);
					
					}
					else{
						//Verification abcisse
						fmpz_mul_ui(tmp,q,2);e=fmpz_get_ui(tmp);
						Psi(a,b,j,Psi1,ctx);fq_poly_pow(poly,Psi1,e,ctx);
						fq_poly_mul(poly,poly,gamma1,ctx);
						
						e=fmpz_get_ui(q);fmpz_sub_ui(tmp,j,1);
						Psi(a,b,tmp,Psi1,ctx);fq_poly_pow(poly1,Psi1,e,ctx);
						fmpz_add_ui(tmp,j,1);Psi(a,b,tmp,Psi1,ctx);
						fq_poly_pow(poly2,Psi1,e,ctx);
						fq_poly_mul(poly1,poly1,poly2,ctx);
						fq_poly_pow(poly2,y2,e,ctx);
						fq_poly_mul(poly1,poly1,poly2,ctx);
						fq_poly_mul(poly1,poly1,teta1,ctx);

						fq_poly_add(absc,poly,poly1,ctx);
						
						//Verification ordonnée
						fmpz_mul_ui(tmp,q,3);e=fmpz_get_ui(tmp);
						Psi(a,b,j,Psi1,ctx);fq_poly_pow(poly,Psi1,e,ctx);
						fq_poly_mul(poly,poly,gamma2,ctx);

						fmpz_sub_ui(tmp,j,1);Psi(a,b,tmp,Psi1,ctx);
						fq_poly_sqr(poly1,Psi1,ctx);
						fmpz_add_ui(tmp,j,2);Psi(a,b,tmp,Psi1,ctx);
						fq_poly_mul(poly1,poly1,Psi1,ctx);
						fmpz_add_ui(tmp,j,1);Psi(a,b,tmp,Psi1,ctx);
						fq_poly_sqr(poly2,Psi1,ctx);
						fmpz_sub_ui(tmp,j,2);
						if(fmpz_cmp_ui(tmp,0)<0){
							fmpz_neg(tmp,tmp);
							Psi(a,b,tmp,Psi1,ctx);
							fq_poly_neg(Psi1,Psi1,ctx);
						}
						else{Psi(a,b,tmp,Psi1,ctx);}
						/*char* var="x";
						fq_poly_print_pretty(Psi1,var,ctx);flint_printf("\n");*/
						fq_poly_mul(poly2,poly2,Psi1,ctx);
						fq_poly_sub(poly1,poly1,poly2,ctx);e=fmpz_get_ui(q);
						fq_poly_mul(poly1,poly1,y2,ctx);
						fq_poly_pow(poly1,poly1,e,ctx);
						fq_poly_mul(poly1,poly1,teta2,ctx);

						fq_poly_sub(ordo,poly,poly1,ctx);
					
					}	
					
				}

				else{
					printf("j=");fmpz_print(j);printf("l=");fmpz_print(l);flint_printf("\n");
					//Calcul de alpha
					fmpz_sub_ui(tmp,q_bar,1);Psi(a,b,tmp,Psi1,ctx);
					fq_poly_sqr(poly, Psi1, ctx);
					fq_poly_mul(poly,poly,y2,ctx);
					fmpz_add_ui(tmp,q_bar,2);Psi(a,b,tmp,Psi1,ctx);
					fq_poly_mul(poly, poly, Psi1, ctx);

					fmpz_add_ui(tmp,q_bar,1);Psi(a,b,tmp,Psi1,ctx);
					fq_poly_sqr(poly1, Psi1, ctx);
					fq_poly_mul(poly1,poly1,y2,ctx);
					fmpz_sub_ui(tmp,q_bar,2);Psi(a,b,tmp,Psi1,ctx);
					fq_poly_mul(poly1, poly1, Psi1, ctx);

					fq_poly_sub(poly, poly, poly1, ctx);

					Psi(a,b,q_bar,Psi1,ctx);
					fq_poly_pow(poly1, Psi1, 3, ctx);
					fmpz_pow_ui(tmp, q, 2);
					fmpz_add_ui(tmp, tmp, 1);
					fmpz_divexact_ui(tmp, tmp, 2);
					fq_poly_pow(alpha, y2, fmpz_get_ui(tmp), ctx);
					fq_poly_mul(alpha, alpha, poly1, ctx);
					fq_set_si(tmp1, 4, ctx);
					fq_poly_scalar_mul_fq(alpha, alpha, tmp1, ctx);

					fq_poly_sub(alpha, poly, alpha, ctx);

					//Calcul de beta
					fmpz_sub_ui(tmp,q_bar,1);Psi(a,b,tmp,Psi1,ctx);
					fmpz_add_ui(tmp,q_bar,1);Psi(a,b,tmp,Psi2,ctx);
					fq_poly_mul(poly, Psi1, Psi2, ctx);
					fq_poly_mul(poly, poly, y2, ctx);
					
					Psi(a,b,q_bar,Psi1,ctx);
					fq_poly_sqr(poly1, Psi1, ctx);
					fq_poly_mul(poly1,poly1,Phi2,ctx);
					
					fq_poly_add(poly, poly1, poly, ctx);

					Psi(a,b,q_bar,Psi1,ctx);
					fq_poly_mul(poly, poly, Psi1, ctx);
					fq_set_si(tmp1, 4, ctx);
					fq_poly_scalar_mul_fq(poly, poly, tmp1, ctx);
					fq_poly_neg(beta,poly,ctx);//beta
					
					//Calcul de gamma1 et teta1
					fmpz_sub_ui(tmp,q_bar,1);Psi(a,b,tmp,Psi1,ctx);
					fmpz_add_ui(tmp,q_bar,1);Psi(a,b,tmp,Psi2,ctx);
					fq_poly_mul(poly,Psi1,Psi2,ctx);
					fq_poly_mul(poly,poly,y2,ctx);
					
					Psi(a,b,q_bar,Psi1,ctx);fq_poly_sqr(poly1,Psi1,ctx);
					fq_poly_mul(poly1,poly1,Phi3,ctx);
					
					fq_poly_sub(poly,poly,poly1,ctx);
					
					fq_poly_sqr(poly1,beta,ctx);
					fq_poly_mul(poly1,poly1,y2,ctx);
					
					fq_poly_mul(poly,poly,poly1,ctx);
					
					Psi(a,b,q_bar,Psi1,ctx);fq_poly_sqr(poly1,Psi1,ctx);
					fq_poly_sqr(poly2,alpha,ctx);
					fq_poly_mul(poly1,poly1,poly2,ctx);
					
					fq_poly_add(gamma1,poly,poly1,ctx);//gamma1

					Psi(a,b,q_bar,Psi1,ctx);
					fq_poly_sqr(poly,Psi1,ctx);
					fq_poly_sqr(poly1,beta,ctx);
					fq_poly_mul(poly1,poly1,y2,ctx);
					fq_poly_mul(teta1,poly,poly1,ctx);//teta1
					
					//Calcul de gamma2 et teta2
					fq_poly_pow(poly,beta,3,ctx);fq_poly_mul(poly,poly,y2,ctx);
					Psi(a,b,q_bar,Psi1,ctx);fq_poly_sqr(poly1,Psi1,ctx);
					fq_poly_mul(teta2,poly,poly1,ctx);//teta2
					
					fq_set_si(tmp1,2,ctx);fmpz_mul(tmp,q,q);e=fmpz_get_ui(tmp);
					fq_poly_set_coeff(Phi4,e,tmp1,ctx);
					fq_set_si(tmp1,1,ctx);fq_poly_set_coeff(Phi4,1,tmp1,ctx);
					
					Psi(a,b,q_bar,Psi1,ctx);
					fq_poly_sqr(poly1,Psi1,ctx);fq_poly_mul(poly,Phi4,poly1,ctx);
					fq_poly_mul(poly,poly,alpha,ctx);fq_poly_sqr(poly1,beta,ctx);
					fq_poly_mul(poly1,poly1,y2,ctx);fq_poly_mul(poly,poly,poly1,ctx);
					
					fq_poly_pow(poly1,alpha,3,ctx);Psi(a,b,q_bar,Psi1,ctx);
					fq_poly_sqr(poly2,Psi1,ctx);fq_poly_mul(poly1,poly1,poly2,ctx);
					
					fq_poly_sub(poly,poly,poly1,ctx);
										
					fmpz_sub_ui(tmp,q_bar,1);Psi(a,b,tmp,Psi1,ctx);
					fmpz_add_ui(tmp,q_bar,1);Psi(a,b,tmp,Psi2,ctx);
					fq_poly_mul(poly1,Psi1,Psi2,ctx);fq_poly_mul(poly1,poly1,alpha,ctx);
					fq_poly_sqr(poly2,beta,ctx);
					fq_poly_mul(poly2,poly2,y2,ctx);fq_poly_mul(poly2,poly2,y2,ctx);
					fq_poly_mul(poly1,poly1,poly2,ctx);
					
					fq_poly_sub(poly,poly,poly1,ctx);
					
					fmpz_mul(tmp,q,q);fmpz_add_ui(tmp,tmp,3);
					fmpz_divexact_ui(tmp,tmp,2);e=fmpz_get_ui(tmp);
					fq_poly_pow(poly1,y2,e,ctx);Psi(a,b,q_bar,Psi1,ctx);
					fq_poly_sqr(poly2,Psi1,ctx);fq_poly_mul(poly1,poly1,poly2,ctx);
					fq_poly_pow(poly2,beta,3,ctx);fq_poly_mul(poly1,poly1,poly2,ctx);
					
					fq_poly_sub(poly,poly,poly1,ctx);
					
					fmpz_sub_ui(tmp, q, 1);fmpz_divexact_ui(tmp, tmp, 2);
					e=fmpz_get_ui(tmp);fq_poly_pow(poly1,y2,e,ctx);
					fq_set_si(tmp1,4,ctx);
					fq_poly_scalar_mul_fq(poly1,poly1,tmp1,ctx);
					
					fq_poly_mul(gamma2,poly,poly1,ctx);//gamma2
					
					if(fmpz_is_even(j)){
						//Verification abcisse
						fmpz_mul_ui(tmp,q,2);e=fmpz_get_ui(tmp);
						Psi(a,b,j,Psi1,ctx);fq_poly_pow(poly,Psi1,e,ctx);
						e=fmpz_get_ui(q);
						fq_poly_pow(poly1,y2,e,ctx);
						fq_poly_mul(poly,poly,poly1,ctx);
						fq_poly_mul(poly,poly,gamma1,ctx);
						
						e=fmpz_get_ui(q);fmpz_sub_ui(tmp,j,1);
						Psi(a,b,tmp,Psi1,ctx);fq_poly_pow(poly1,Psi1,e,ctx);
						fmpz_add_ui(tmp,j,1);Psi(a,b,tmp,Psi1,ctx);
						fq_poly_pow(poly2,Psi1,e,ctx);
						fq_poly_mul(poly1,poly1,poly2,ctx);
						fq_poly_mul(poly1,poly1,teta1,ctx);

						fq_poly_add(absc,poly,poly1,ctx);
						
						//Verification ordonnée
						fmpz_mul_ui(tmp,q,3);e=fmpz_get_ui(tmp);
						Psi(a,b,j,Psi1,ctx);fq_poly_pow(poly,Psi1,e,ctx);
						fmpz_mul_ui(tmp,q,3);fmpz_sub_ui(tmp,tmp,1);
						fmpz_divexact_ui(tmp,tmp,2);e=fmpz_get_ui(tmp);
						fq_poly_pow(poly1,y2,e,ctx);
						fq_poly_mul(poly,poly,poly1,ctx);
						fq_poly_mul(poly,poly,gamma2,ctx);

						fmpz_sub_ui(tmp,j,1);Psi(a,b,tmp,Psi1,ctx);
						fq_poly_sqr(poly1,Psi1,ctx);
						fmpz_add_ui(tmp,j,2);Psi(a,b,tmp,Psi1,ctx);
						fq_poly_mul(poly1,poly1,Psi1,ctx);
						fmpz_add_ui(tmp,j,1);Psi(a,b,tmp,Psi1,ctx);
						fq_poly_sqr(poly2,Psi1,ctx);
						fmpz_sub_ui(tmp,j,2);
						if(fmpz_cmp_ui(tmp,0)<0){
							fmpz_neg(tmp,tmp);
							Psi(a,b,tmp,Psi1,ctx);
							fq_poly_neg(Psi1,Psi1,ctx);
						}
						else{Psi(a,b,tmp,Psi1,ctx);}
						/*char* var="x";
						fq_poly_print_pretty(Psi1,var,ctx);flint_printf("\n");*/
						fq_poly_mul(poly2,poly2,Psi1,ctx);
						fq_poly_sub(poly1,poly1,poly2,ctx);e=fmpz_get_ui(q);
						fq_poly_pow(poly1,poly1,e,ctx);fq_poly_mul(poly1,poly1,teta2,ctx);
						fmpz_sub_ui(tmp,q,1);fmpz_divexact_ui(tmp,tmp,2);
						e=fmpz_get_ui(tmp);fq_poly_pow(poly2,y2,e,ctx);
						fq_poly_mul(poly1,poly1,poly2,ctx);
						
						fq_poly_sub(ordo,poly,poly1,ctx);
						fq_poly_mul(ordo,ordo,y2,ctx);
					
					}
					else{
						//Verification abcisse
						fmpz_mul_ui(tmp,q,2);e=fmpz_get_ui(tmp);
						Psi(a,b,j,Psi1,ctx);fq_poly_pow(poly,Psi1,e,ctx);
						fq_poly_mul(poly,poly,gamma1,ctx);
						
						e=fmpz_get_ui(q);fmpz_sub_ui(tmp,j,1);
						Psi(a,b,tmp,Psi1,ctx);fq_poly_pow(poly1,Psi1,e,ctx);
						fmpz_add_ui(tmp,j,1);Psi(a,b,tmp,Psi1,ctx);
						fq_poly_pow(poly2,Psi1,e,ctx);
						fq_poly_mul(poly1,poly1,poly2,ctx);
						fq_poly_pow(poly2,y2,e,ctx);
						fq_poly_mul(poly1,poly1,poly2,ctx);
						fq_poly_mul(poly1,poly1,teta1,ctx);

						fq_poly_add(absc,poly,poly1,ctx);
						
						//Verification ordonnée
						fmpz_mul_ui(tmp,q,3);e=fmpz_get_ui(tmp);
						Psi(a,b,j,Psi1,ctx);fq_poly_pow(poly,Psi1,e,ctx);
						fq_poly_mul(poly,poly,gamma2,ctx);

						fmpz_sub_ui(tmp,j,1);Psi(a,b,tmp,Psi1,ctx);
						fq_poly_sqr(poly1,Psi1,ctx);
						fmpz_add_ui(tmp,j,2);Psi(a,b,tmp,Psi1,ctx);
						fq_poly_mul(poly1,poly1,Psi1,ctx);
						fmpz_add_ui(tmp,j,1);Psi(a,b,tmp,Psi1,ctx);
						fq_poly_sqr(poly2,Psi1,ctx);
						fmpz_sub_ui(tmp,j,2);
						if(fmpz_cmp_ui(tmp,0)<0){
							fmpz_neg(tmp,tmp);
							Psi(a,b,tmp,Psi1,ctx);
							fq_poly_neg(Psi1,Psi1,ctx);
						}
						else{Psi(a,b,tmp,Psi1,ctx);}
						/*char* var="x";
						fq_poly_print_pretty(Psi1,var,ctx);flint_printf("\n");*/
						fq_poly_mul(poly2,poly2,Psi1,ctx);
						fq_poly_sub(poly1,poly1,poly2,ctx);e=fmpz_get_ui(q);
						fq_poly_mul(poly1,poly1,y2,ctx);
						fq_poly_pow(poly1,poly1,e,ctx);
						fq_poly_mul(poly1,poly1,teta2,ctx);

						fq_poly_sub(ordo,poly,poly1,ctx);
					}
				}
			
			
			/*fmpz_one(j);
			fmpz_add_ui(tmp,l,1);fmpz_divexact_ui(tmp,tmp,2);
			while(fmpz_cmp(j,tmp)<=0){
				if(fmpz_is_even(j)){
					//Verification abcisse
					fmpz_mul_ui(tmp,q,2);e=fmpz_get_ui(tmp);
					Psi(a,b,j,Psi1,ctx);fq_poly_pow(poly,Psi1,e,ctx);
					e=fmpz_get_ui(q);
					fq_poly_pow(poly1,y2,e,ctx);
					fq_poly_mul(poly,poly,poly1,ctx);
					fq_poly_mul(poly,poly,gamma1,ctx);
					
					e=fmpz_get_ui(q);fmpz_sub_ui(tmp,j,1);
					Psi(a,b,tmp,Psi1,ctx);fq_poly_pow(poly1,Psi1,e,ctx);
					fmpz_add_ui(tmp,j,1);Psi(a,b,tmp,Psi1,ctx);
					fq_poly_pow(poly2,Psi1,e,ctx);
					fq_poly_mul(poly1,poly1,poly2,ctx);
					fq_poly_mul(poly1,poly1,teta1,ctx);

					fq_poly_sub(absc,poly,poly1,ctx);
					
					//Verification ordonnée
					fmpz_mul_ui(tmp,q,3);e=fmpz_get_ui(tmp);
					Psi(a,b,j,Psi1,ctx);fq_poly_pow(poly,Psi1,e,ctx);
					fmpz_mul_ui(tmp,q,3);fmpz_sub_ui(tmp,tmp,1);
					fmpz_divexact_ui(tmp,tmp,2);e=fmpz_get_ui(tmp);
					fq_poly_pow(poly1,y2,e,ctx);
					fq_poly_mul(poly,poly,poly1,ctx);
					fq_poly_mul(poly,poly,gamma2,ctx);

					fmpz_sub_ui(tmp,j,1);Psi(a,b,tmp,Psi1,ctx);
					fq_poly_sqr(poly1,Psi1,ctx);
					fmpz_add_ui(tmp,j,2);Psi(a,b,tmp,Psi1,ctx);
					fq_poly_mul(poly1,poly1,Psi1,ctx);
					fmpz_add_ui(tmp,j,1);Psi(a,b,tmp,Psi1,ctx);
					fq_poly_sqr(poly2,Psi1,ctx);
					fmpz_sub_ui(tmp,j,2);
					if(fmpz_cmp_ui(tmp,0)<0){
						fmpz_neg(tmp,tmp);
						Psi(a,b,tmp,Psi1,ctx);
						fq_poly_neg(Psi1,Psi1,ctx);
					}
					else{Psi(a,b,tmp,Psi1,ctx);}
					char* var="x";
					fq_poly_print_pretty(Psi1,var,ctx);flint_printf("\n");
					fq_poly_mul(poly2,poly2,Psi1,ctx);
					fq_poly_sub(poly1,poly1,poly2,ctx);e=fmpz_get_ui(q);
					fq_poly_pow(poly1,poly1,e,ctx);fq_poly_mul(poly1,poly1,teta2,ctx);

					fq_poly_sub(ordo,poly,poly1,ctx);
					
				}
				else{
					//Verification abcisse
					fmpz_mul_ui(tmp,q,2);e=fmpz_get_ui(tmp);
					Psi(a,b,j,Psi1,ctx);fq_poly_pow(poly,Psi1,e,ctx);
					fq_poly_mul(poly,poly,gamma1,ctx);
					
					e=fmpz_get_ui(q);fmpz_sub_ui(tmp,j,1);
					Psi(a,b,tmp,Psi1,ctx);fq_poly_pow(poly1,Psi1,e,ctx);
					fmpz_add_ui(tmp,j,1);Psi(a,b,tmp,Psi1,ctx);
					fq_poly_pow(poly2,Psi1,e,ctx);
					fq_poly_mul(poly1,poly1,poly2,ctx);
					e=fmpz_get_ui(q);
					fq_poly_pow(poly2,y2,e,ctx);
					fq_poly_mul(poly1,poly1,poly2,ctx);
					fq_poly_mul(poly1,poly1,teta1,ctx);

					fq_poly_sub(absc,poly,poly1,ctx);
					
					//Verification ordonnée
					fmpz_mul_ui(tmp,q,3);e=fmpz_get_ui(tmp);
					Psi(a,b,j,Psi1,ctx);fq_poly_pow(poly,Psi1,e,ctx);
					fq_poly_mul(poly,poly,gamma2,ctx);

					fmpz_sub_ui(tmp,j,1);Psi(a,b,tmp,Psi1,ctx);
					fq_poly_sqr(poly1,Psi1,ctx);
					fmpz_add_ui(tmp,j,2);Psi(a,b,tmp,Psi1,ctx);
					fq_poly_mul(poly1,poly1,Psi1,ctx);
					fmpz_add_ui(tmp,j,1);Psi(a,b,tmp,Psi1,ctx);
					fq_poly_sqr(poly2,Psi1,ctx);
					fmpz_sub_ui(tmp,j,2);
					if(fmpz_cmp_ui(tmp,0)<0){
						fmpz_neg(tmp,tmp);
						Psi(a,b,tmp,Psi1,ctx);
						fq_poly_neg(Psi1,Psi1,ctx);
					}
					else{Psi(a,b,tmp,Psi1,ctx);}
					fq_poly_mul(poly2,poly2,Psi1,ctx);
					fq_poly_sub(poly1,poly1,poly2,ctx);e=fmpz_get_ui(q);
					fq_poly_mul(poly1,poly1,y2,ctx);
					fq_poly_pow(poly1,poly1,e,ctx);
					fq_poly_mul(poly1,poly1,teta2,ctx);

					fq_poly_sub(ordo,poly,poly1,ctx);
				}*/
									
				//Calcul du pgcd
				Psi(a,b,l,Psi2,ctx);
				fq_poly_gcd(poly,absc,Psi2,ctx);fq_poly_gcd(poly1,ordo,Psi2,ctx);
				
				char* var="x";
				//fq_poly_print_pretty(absc,var,ctx);printf("l=");fmpz_print(l);
				fq_poly_print_pretty(poly,var,ctx);printf("l=");fmpz_print(l);flint_printf("\n");
				//fq_poly_print_pretty(ordo,var,ctx);printf("l=");fmpz_print(l);
				fq_poly_print_pretty(poly1,var,ctx);printf("l=");fmpz_print(l);flint_printf("\n");
					
				if((!fq_poly_is_one(poly,ctx))){
					if((!fq_poly_is_one(poly1,ctx))){
						fmpz_CRT(t, t, c, j, l, 1);
						fmpz_print(tmp);printf("mod");fmpz_print(l);flint_printf("\n");
					}
					else{
						fmpz_neg(tmp2,j);
						fmpz_CRT(t, t, c, tmp2, l, 1);
						fmpz_print(tmp2);printf("mod");fmpz_print(l);flint_printf("\n");
					}
					break;
				}
								
				fmpz_add_ui(j,j,1);
			}
			
		}
		
		fmpz_mul(c,c,l);
		fmpz_print(l);flint_printf("\n");
		fnext_prime(l);
		if(fmpz_cmp(l,fq_ctx_prime(ctx))==0){fnext_prime(l);}
	}
	
	fmpz_print(t);flint_printf("\n");//La trace de Froebenius
	
	fmpz_add_ui(tmp,q,1);fmpz_sub(tmp,tmp,t);
	fmpz_print(tmp);flint_printf("\n");//Le cardinal
	
	fq_clear(tmp1,ctx);fmpz_clear(l);fmpz_clear(c);
	fmpz_clear(q_sqrt);fmpz_clear(tmp);fmpz_clear(tmp2);fmpz_clear(tmp3);
	fmpz_clear(t);fmpz_clear(p);fmpz_clear(j);fmpz_clear(q_bar);
	fmpz_clear(w);fq_poly_clear(y2,ctx);
	fq_poly_clear(Phi1,ctx);fq_poly_clear(Phi2,ctx);
	fq_poly_clear(Phi3,ctx);fq_poly_clear(Phi4,ctx);fq_poly_clear(poly,ctx);
	fq_poly_clear(poly1,ctx);fq_poly_clear(poly2,ctx);
	fq_poly_clear(Psi1,ctx);fq_poly_clear(Psi2,ctx);
	fq_poly_clear(beta,ctx);fq_poly_clear(alpha,ctx);
	fq_poly_clear(teta1,ctx);fq_poly_clear(teta2,ctx);
	fq_poly_clear(gamma1,ctx);fq_poly_clear(gamma2,ctx);
	fq_poly_clear(absc,ctx);fq_poly_clear(ordo,ctx);

}

int main(){
	fmpz_t p;fmpz_init(p);fmpz_set_si(p,131);
	signed long d = 1;char *var="x";fq_ctx_t ctx;
	fq_ctx_init(ctx, p, d, var);
	
	fq_t a,b;fq_init(a,ctx);fq_init(b,ctx);
	fq_set_si(a,100,ctx);fq_set_si(b,151,ctx);
	
	/*
	fmpz_t k;fmpz_init(k);fmpz_set_si(k,4);
	fq_poly_t Psi1;fq_poly_init(Psi1,ctx);
	Psi(a,b,k,Psi1,ctx);
	fq_poly_print_pretty(Psi1,var,ctx);
	flint_printf("\n");*/
	
	schoof(a,b,ctx,p);
	
	return 0;
}

