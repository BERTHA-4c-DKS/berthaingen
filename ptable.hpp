#ifndef _BERTHAINGEN_PTABLE_INC_
#define _BERTHAINGEN_PTABLE_INC_

namespace berthaingen
{
  class ptable
  {
    public:
      enum element 
      {
         H, 
         HE, 
         LI, 
         BE, 
         B,  
         C,  
         N,  
         O,  
         F,  
         NE, 
         NA, 
         MG, 
         AL, 
         SI, 
         P,  
         S,  
         CL, 
         AR, 
         K,  
         CA, 
         SC, 
         TI, 
         V,  
         CR, 
         MN, 
         FE, 
         CO, 
         NI, 
         CU, 
         ZN, 
         GA, 
         GE, 
         AS, 
         SE, 
         BR, 
         KR, 
         RB, 
         SR, 
         Y,  
         ZR, 
         NB, 
         MO, 
         TC, 
         RU, 
         RH, 
         PD, 
         AG, 
         CD, 
         IN, 
         SN, 
         SB, 
         TE, 
         I,  
         XE, 
         CS, 
         BA, 
         LA, 
         CE, 
         PR, 
         ND, 
         PM, 
         SM, 
         EU, 
         GD, 
         TB, 
         DY, 
         HO, 
         ER, 
         TM, 
         YB, 
         LU, 
         HF, 
         TA, 
         W,  
         RE, 
         OS, 
         IR, 
         PT, 
         AU, 
         HG, 
         TL, 
         PB, 
         BI, 
         PO, 
         AT, 
         RN, 
         FR, 
         RA, 
         AC, 
         TH, 
         PA, 
         U,  
         NP, 
         PU, 
         AM, 
         CM, 
         BK, 
         CF, 
         ES, 
         FM, 
         MD, 
         NO, 
         LR, 
         RF, 
         DB, 
         SG, 
         BH, 
         HS, 
         MT, 
         DS, 
         RG, 
         CN, 
         NH, 
         FL, 
         MC, 
         LV, 
         TS, 
         OG, 
         NO_ELEMENT
      };

      static int atomic_number (element);

      static double atomic_weight (element);

      static const char * atomic_symbol (element);

      static const char * element_name (element);

      static element symbol_to_element (const char *);

      static int maxl (element);
  };
}

#endif

