#ifndef PRMS
#define PRMS

#include <vector>
#include <math.h>


using namespace std;

// the i_phi parameters are the relative infectiousness of individuals in that state; the infectiousness of the I1 to I4 classes is assumed to be 1.0
//

enum parameter_index {i_N, i_beta, i_startday_beta, i_endday_beta, i_beta_hosp, i_prelockdown_beta, 
                      i_min_relbeta_00, i_min_relbeta_10, i_min_relbeta_20, i_min_relbeta_30, i_min_relbeta_40, i_min_relbeta_50, i_min_relbeta_60, i_min_relbeta_70, i_min_relbeta_80, 
                      i_beta_icu, i_beta_vent, i_len_incub_period, i_len_symptomatic_infectious_period_phase_1, i_len_symptomatic_infectious_period_phase_2, i_len_medicalfloor_hospital_stay, 
                      i_phi_asymp, i_phi_incub, i_phi_symp_phase2, i_phi_hosp, i_phi_hosp_recovering, i_phi_icu, i_phi_vent, i_selfisolation_factor, i_mean_time_vent, 
                      i_phi_vac1_protect_duration, i_phi_vac2_protect_duration, i_vac1_phase1_dpd, i_vac2_phase1_dpd,
                      i_vac1_eff_halflife, i_vac1_eff_slope, i_vac2_eff_halflife, i_vac2_eff_slope,
                      i_vac1_eff_halflife_pow_slope, i_vac2_eff_halflife_pow_slope, 
                      i_vac1_foi, i_vac2_foi,
                      i_len_natural_immunity, i_reporting_rate,
                      num_params}; 

enum contact_matrix_loc {be, uk, ukbe, num_contact_matrix_loc};

typedef enum parameter_index prm_index;


class prms
{   
public:    
    explicit prms();          	// constructor
    ~prms();         	// destructor


    vector<double> v;			// this holds all the parameters; they are indexed by the enums above

    //BEGIN --- FOR ALL THE VECTORS BELOW, THERE WILL BE NUMAC ENTRIES FOR THE NINE AGE CLASSES
    //
    
    vector<double> v_rel_susc;              // relative susceptibility of individuals in different age groups; reference age group is 30-39
    // vector<double> v_mixing_level;          // relative mixing level per age group (i.e. leaving home; contact numbers; but irrespective of ages of contacts); reference age group is 0-9
    // vector<double> v_mixing_level_postld;   // relative mixing level per age group after the lockdown (hard coded date is April 30 2020)
    
    vector<double> v_prob_E_A;          // fraction of exposed individuals who progress to asymptomatic infection
    
    vector<double> v_prob_I2_H;         // fraction of infected & symp individuals who are hospitalized after class I_2
    //vector<double> v_fraction_crit;   // DEPRECATED fraction of infected & symp individuals who are immediately admitted to critical care after class I_2
    
    vector<double> v_prob_I4_D;         // probability of death for a non-hospitalized person
    
    vector<double> v_prob_HA4_D;        // probability of death on the regular medical-level-of-care hospital floor
    vector<double> v_prob_HA_CA;        // probability of progressing to the ICU from any of the HA-classes during the acute-phase hospitalization phase
    
    vector<double> v_prob_V_D;          // probability of death if you are on a ventilator

    
    vector<double> v_prob_CA_D;         // probability of death directly from the CA-class (critical care, acute phase, but not on ventilator)
    vector<double> v_prob_CR_D;         // probability of death directly from the CR-class (critical care, post-ventilation)
    vector<double> v_prob_CA_V;         // probability of progressing to mechanical ventilation from the CA-class 
                                        // if you take 'one minus' the two probabilities above, you will get the probability that someone moves from CA to HR
    
    
    // vector<double> v_prob_S_Z_1;          // fraction of susceptibles being vaccinated (age-stratified)
    vector<double> v_number_to_Z_1_phase1; // the number of individuals in each age group who are vaccinated per day during phase 1 (vaccine rollout)
    vector<double> v_already_moved_to_Z_1_phase1;  // the number of individuals in each age group who have been vaccinated out of the corresponding value in  v_number_to_Z_1_phase1
    vector<double> v_efficacy_Z_1;        // efficacy of vaccine 1 at each stage in NUMZ_1 stages
    vector< vector<double> > v_rel_sus_Z_1;         // relative susceptibility at each stage in NUMZ_1 stages for each age group
    vector<double> v_rel_eff_Z_1;         // relative efficacy for each age group; default is 1.0 for everyone

    vector<int> v_begin_days_phase1_vac1; // time-varying campaign
    vector<int> v_end_days_phase1_vac1;
    vector<int> v_dpd_phase1_vac1;
    vector< vector<double> > v_vac_ratios_phase1_vac1;
    vector< vector<double> > v_vac_fracs_phase1_vac1;
    int index_current_day_phase1_vac1;
    
    // vector<double> v_prob_S_Z_2;          // fraction of susceptibles being vaccinated (age-stratified)
    // vector<double> v_number_S_Z_2_phase1; // the number of individuals in each age group who are vaccinated per day during phase 1 (vaccine rollout)
    // vector<double> v_efficacy_Z_2;        // efficacy of vaccine 2 at each stage in NUMZ_2 stages
    // vector< vector<double> > v_rel_sus_Z_2;         // relative susceptibility at each stage in NUMZ_2 stages for each age group
    // vector<double> v_rel_eff_Z_2;         // relative efficacy for each age group; default is 1.0 for everyone

    int cm_loc = (int)(contact_matrix_loc::be); // default Belgium


    vector<double> v_pop_frac;        // population percentage breakdown by age group
    

    //
    //END --- FOR ALL THE VECTORS ABOVE, THERE ARE NUMAC ENTRIES FOR THE NINE AGE CLASSES
    
    
    // NOTE THE TWO ARRAYS BELOW MUST BE OF THE SAME LENGTH
    //      THE "BETATIME" IS THE START-TIME THAT THE CORRESPONDING BETA COMES INTO EFFECT
    //      THE LAST BETA STAYS IN EFFECT FOREVER ONCE WE HAVE PASSED THE LAST "BETA-TIME"
    vector<double> v_betas;             // these are all the different beta parameters that will change every T time steps
                                        // where the goal is to have T=1 so you have beta changing daily
                                        
    vector<double> v_betatimes;         // these are all the different times that the beta parameters change
    
    
    
    bool earlymarch_highhosp_period;    // are we in the period in early March when hospitalization rates were higher than normal?
    double earlymarch_highhosp_factor;
    double earlymarch_highhosp_endday;
    
    void apply_earlymarch_hosprates( void );
    void end_earlymarch_hosprates( void );
    
    int index_current_beta;
    
    
    void assign_new_beta( void );
    double get_new_update_time( void );


    int Increase_index_current_day_phase1_vac1();
    
};

#endif // PRMS
