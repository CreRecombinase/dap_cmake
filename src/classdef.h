#ifndef __CLASSDEF_H_
#define __CLASSDEF_H_

#include <stdlib.h>
#include <string>
#include <string.h>
#include <vector>
#include <map>

#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>



namespace torus {



class SNP {

 public:

  std::string id;
  int index;
  double log10_BF;
  int dtss_bin; // distance to TSS, used by fastQTL

  SNP(std::string snp_id, double snp_log10_BF, int snp_index){
    id = snp_id;
    log10_BF = snp_log10_BF;
    index = snp_index;
  }
 
    
 
};





class Locus {
  
 public:
  
  // possible gene information/annotation
  std::string id;
  
  std::vector<SNP> snpVec;
  
  gsl_vector *prior_vec;
  gsl_vector *pip_vec;
  
  double log10_lik; // log10 of marginal likelihood
  
  double fdr;
  
  
  Locus(std::string locus_id,  std::vector<SNP> & snpVec_){ id = locus_id;  snpVec = snpVec_; prior_vec = pip_vec =0;  };
  Locus(){};

  
  void EM_update();
  void compute_fdr();


};






class controller {

 public:
 
  controller(){
    p=kc=kd=dist_bin_level = 0;
    force_logistic = 0;
    dist_bin_size = -1;
    fastqtl_use_dtss = 0;
    dist_bin = 0;
    finish_em = 0;
    single_fuzzy_annot = 0;
    l1_lambda=l2_lambda=0;
    init_pi1 = 1e-3;
    print_avg = 0;
  }

  // storage
  std::vector<Locus> locVec;
  
  
  int p; // number of loc-SNP pairs
  
  int kc; // number of continuous covariate
  int kd; // number of discrete covariate


  double dist_bin_size;


  std::map<std::string,int> loc_hash;
  std::map<std::string,int> snp_hash;
  std::vector<std::string> cvar_name_vec;
  std::vector<std::string> dvar_name_vec;



  
  gsl_vector_int *dist_bin;
  std::map<int, int> dtss_map;
  std::map<int, int> dtss_rmap;
  int dist_bin_level;
  double EM_thresh;
  
  gsl_vector_int *dlevel; // (kd+1) entry levels of each factor


  gsl_matrix *Xc;  // p x kc
  gsl_matrix_int *Xd; // p x kd
  
  
  gsl_vector *prior_vec;
  gsl_vector *pip_vec;
  gsl_vector *beta_vec;
  
  int ncoef;

  double final_log10_lik;
  

  double init_pi1;
  int nthread; 


  int finish_em;

  int fastqtl_use_dtss;
  int print_avg;


  void load_data(char *filename);    // load data with MatrixeQTL format -- default
  void load_data_zscore(char *filename); // load data with z-score/t-score
  void load_data_BF(char *filename); // load data with pre-computed log10 Bayes factors
  void load_data_fastqtl(char *filename); //load data with fastQTL format


  void load_map(char *gene_map_file, char *snp_map_file);
  void load_annotation(char *annot_file);
  int count_factor_level(int col);
  
  void simple_regression();
  void single_ct_regression();
  void single_probt_est();
  void single_probt_regression();
  int force_logistic;
  int single_fuzzy_annot;
  
  double l1_lambda, l2_lambda; //shrinkage for enrich param est


  void init_params();
    
  void run_EM();

  
  void find_eGene(const char* qtl_file,double thresh=0.05);
  void estimate();
  void dump_prior(char *path);
  void dump_pip(char *file);


 private:
  double eval_likelihood(double x, int index);
  double fine_optimize_beta(int index, double est, double null_log10_lik, double &curr_log10_lik);
  



};




double log10_weighted_sum(std::vector<double> & val_vec, std::vector<double> & wts_vec);
double compute_log10_BF(double beta, double se_beta);
double compute_log10_BF(double z_score);
bool   rank_by_fdr (const Locus & loc1 , const Locus & loc2);
  
int classify_dist_bin(int snp_pos, int tss, double bin_size = -1);
double map_bin_2_dist(int bin, double bin_size=-1);


}
#endif
