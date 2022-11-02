/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 The Boeing Company
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Authors: Gary Pei <guangyu.pei@boeing.com>
 *
 * Modified by: 
 * Lesly Maygua <lesly.maygua@epn.edu.ec>
 * Luis Urquiza <luis.urquiza@epn.edu.ec>
 * Alejandro Flores <xalejandroflores@gmail.com>
 */

#include <cmath>
#include "error-rate-model4.h"
#include "wifi-phy.h"
#include "ns3/log.h"


namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("ErrorRateModel4");

NS_OBJECT_ENSURE_REGISTERED (ErrorRateModel4);

TypeId
ErrorRateModel4::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ErrorRateModel4")
    .SetParent<ErrorRateModel> ()
    .SetGroupName ("Wifi")
    .AddConstructor<ErrorRateModel4> ()
  ;
  return tid;
}

ErrorRateModel4::ErrorRateModel4 ()

//Begin added by juan
: modelType (15)

//end added by juan
{
}


//==begin added by juan

void
ErrorRateModel4::SetErrorModel(uint32_t _model)
{
  modelType = _model;
}
uint32_t
ErrorRateModel4::GetModel (void) const
{
  return modelType;
}

//===addded by juan


double
ErrorRateModel4::GetBpskBer (double snr) const
{
  NS_LOG_FUNCTION (this << snr);
  double z = std::sqrt (snr);
  double ber = 0.5 * erfc (z);
  NS_LOG_INFO ("bpsk snr=" << snr << " ber=" << ber);
  return ber;
}

double
ErrorRateModel4::GetQpskBer (double snr) const
{
  NS_LOG_FUNCTION (this << snr);
  double z = std::sqrt (snr / 2.0);
  double ber = 0.5 * erfc (z);
  NS_LOG_INFO ("qpsk snr=" << snr << " ber=" << ber);
  return ber;
}

double
ErrorRateModel4::Get16QamBer (double snr) const
{
  NS_LOG_FUNCTION (this << snr);
  double z = std::sqrt (snr / (5.0 * 2.0));
  double ber = 0.75 * 0.5 * erfc (z);
  NS_LOG_INFO ("16-Qam" << " snr=" << snr << " ber=" << ber);
  return ber;
}

double
ErrorRateModel4::Get64QamBer (double snr) const
{
  NS_LOG_FUNCTION (this << snr);
  double z = std::sqrt (snr / (21.0 * 2.0));
  double ber = 7.0 / 12.0 * 0.5 * erfc (z);
  NS_LOG_INFO ("64-Qam" << " snr=" << snr << " ber=" << ber);
  return ber;
}
double
ErrorRateModel4::Get256QamBer (double snr) const
{
  NS_LOG_FUNCTION (this << snr);
  double z = std::sqrt (snr / (85.0 * 2.0));
  double ber = 15.0 / 32.0 * 0.5 * erfc (z);
  NS_LOG_INFO ("256-Qam" << " snr=" << snr << " ber=" << ber);
  return ber;
}

double
ErrorRateModel4::GetFecBpskBer (double snr, uint32_t nbits,
                                   uint32_t bValue) const
{
  NS_LOG_FUNCTION (this << snr << nbits << bValue);
  double ber = GetBpskBer (snr);
  if (ber == 0.0)
    {
      return 1.0;
    }
  double pe = CalculatePe (ber, bValue);
  pe = std::min (pe, 1.0);
  double pms = std::pow (1 - pe, (double)nbits);
  return pms;
}

double
ErrorRateModel4::GetFecQpskBer (double snr, uint32_t nbits,
                                   uint32_t bValue) const
{
  NS_LOG_FUNCTION (this << snr << nbits << bValue);
  double ber = GetQpskBer (snr);
  if (ber == 0.0)
    {
      return 1.0;
    }
  double pe = CalculatePe (ber, bValue);
  pe = std::min (pe, 1.0);
  double pms = std::pow (1 - pe, (double)nbits);
  return pms;
}

double
ErrorRateModel4::CalculatePe (double p, uint32_t bValue) const
{
  NS_LOG_FUNCTION (this << p << bValue);
  double D = std::sqrt (4.0 * p * (1.0 - p));
  double pe = 1.0;
  if (bValue == 1)
    {
      //code rate 1/2, use table 3.1.1
      pe = 0.5 * (36.0 * std::pow (D, 10)
                  + 211.0 * std::pow (D, 12)
                  + 1404.0 * std::pow (D, 14)
                  + 11633.0 * std::pow (D, 16)
                  + 77433.0 * std::pow (D, 18)
                  + 502690.0 * std::pow (D, 20)
                  + 3322763.0 * std::pow (D, 22)
                  + 21292910.0 * std::pow (D, 24)
                  + 134365911.0 * std::pow (D, 26));
    }
  else if (bValue == 2)
    {
      //code rate 2/3, use table 3.1.2
      pe = 1.0 / (2.0 * bValue) *
        (3.0 * std::pow (D, 6)
         + 70.0 * std::pow (D, 7)
         + 285.0 * std::pow (D, 8)
         + 1276.0 * std::pow (D, 9)
         + 6160.0 * std::pow (D, 10)
         + 27128.0 * std::pow (D, 11)
         + 117019.0 * std::pow (D, 12)
         + 498860.0 * std::pow (D, 13)
         + 2103891.0 * std::pow (D, 14)
         + 8784123.0 * std::pow (D, 15));
    }
  else if (bValue == 3)
    {
      //code rate 3/4, use table 3.1.2
      pe = 1.0 / (2.0 * bValue) *
        (42.0 * std::pow (D, 5)
         + 201.0 * std::pow (D, 6)
         + 1492.0 * std::pow (D, 7)
         + 10469.0 * std::pow (D, 8)
         + 62935.0 * std::pow (D, 9)
         + 379644.0 * std::pow (D, 10)
         + 2253373.0 * std::pow (D, 11)
         + 13073811.0 * std::pow (D, 12)
         + 75152755.0 * std::pow (D, 13)
         + 428005675.0 * std::pow (D, 14));
    }
  else if (bValue == 5)
    {
      //code rate 5/6, use table V from D. Haccoun and G. Begin, "High-Rate Punctured Convolutional Codes
      //for Viterbi Sequential Decoding", IEEE Transactions on Communications, Vol. 32, Issue 3, pp.315-319.
      pe = 1.0 / (2.0 * bValue) *
        (92.0 * std::pow (D, 4.0)
         + 528.0 * std::pow (D, 5.0)
         + 8694.0 * std::pow (D, 6.0)
         + 79453.0 * std::pow (D, 7.0)
         + 792114.0 * std::pow (D, 8.0)
         + 7375573.0 * std::pow (D, 9.0)
         + 67884974.0 * std::pow (D, 10.0)
         + 610875423.0 * std::pow (D, 11.0)
         + 5427275376.0 * std::pow (D, 12.0)
         + 47664215639.0 * std::pow (D, 13.0));
    }
  else
    {
      NS_ASSERT (false);
    }
  return pe;
}

double
ErrorRateModel4::GetFec16QamBer (double snr, uint32_t nbits,
                                    uint32_t bValue) const
{
  NS_LOG_FUNCTION (this << snr << nbits << bValue);
  double ber = Get16QamBer (snr);
  if (ber == 0.0)
    {
      return 1.0;
    }
  double pe = CalculatePe (ber, bValue);
  pe = std::min (pe, 1.0);
  double pms = std::pow (1 - pe, static_cast<double> (nbits));
  return pms;
}

double
ErrorRateModel4::GetFec64QamBer (double snr, uint32_t nbits,
                                    uint32_t bValue) const
{
  NS_LOG_FUNCTION (this << snr << nbits << bValue);
  double ber = Get64QamBer (snr);
  if (ber == 0.0)
    {
      return 1.0;
    }
  double pe = CalculatePe (ber, bValue);
  pe = std::min (pe, 1.0);
  double pms = std::pow (1 - pe, static_cast<double> (nbits));
  return pms;
}

double
ErrorRateModel4::GetFec256QamBer (double snr, uint32_t nbits,
                                     uint32_t bValue) const
{
  NS_LOG_FUNCTION (this << snr << nbits << bValue);
  double ber = Get256QamBer (snr);
  if (ber == 0.0)
    {
      return 1.0;
    }
  double pe = CalculatePe (ber, bValue);
  pe = std::min (pe, 1.0);
  double pms = std::pow (1 - pe, static_cast<double> (nbits));
  return pms;
}


//New error model adaptation based on paper
double
ErrorRateModel4::GetChunkSuccessRate (WifiMode mode, WifiTxVector txVector, double snr, uint32_t nbits) const
{


	//std::cout << GetModel() <<"\n";

  double *pt;

	double arreglo_modelo[8][8] = {
			1.46306201857628,5.95561541100306,0.226237282504625, -1.71546315817952,-0.00159889753416400,-1.45219857140000e-05,-0.00322724871769100,6.26992610800000e-06,
			2.20028804328588,-2.75913894519549, -0.188331121689950,-2.08712268055257,-0.00213367060337000,3.09509733460000e-05,-0.00163519839060700,-1.11858017600000e-06,
			2.66333131156803,-0.006674305540532, 0.217977092689326,-1.71463577288853,-0.000835317397577000,0.000359398121850000,-0.00272304331280200,6.20234037600000e-06,
			1.97982127104145,-8.97158192001013, -0.197326319094122,-2.06646474517010,-0.00493153190533500,9.86025214900000e-06,-0.000765512350149000,-1.04553919300000e-06,
			3.60490590203545,-8.76893665712924, 0.236697472606611,-1.49445844224010,-0.00194957082428200,1.73400133600000e-05,-0.00237670252549700,6.64486162700000e-06,
			1.10714954194030,-19.1444727243866, -0.109834362737900,-1.76045729768401,-0.00475151946507600,3.00690143200000e-06,-0.00143651704417000,-2.12367667200000e-06,
			2.74618227880884,-25.0754769976109, 4.44889674208461,-6.07833355891381,-0.000997162826614000,-2.46007936000000e-07,-3.35392409560000e-05,-2.39168030240000e-05,
			3.98019415655143,-30.4639101990618, 0.106644432467539,-1.81439791918775,-0.00204614663164000,-2.19533346000000e-06,-0.00219070536632800,-5.68746732600000e-06
	};

	//(10 - en adelante)

	double arreglo_modelo1[8][8] = {
			1.32734904726860,5.61477605584785,0.222385147011877, -1.63296289924399,-0.00224003619294600,-1.40917843820000e-05,-0.00152375817313500,7.56246945100000e-06,
			2.20070014272456,-2.36833721156488, 0.142769968699474,-1.78863769462741,-0.00146909455800900,3.75149414450000e-05,-0.000663182202450000,5.14409391100000e-06,
			2.48667827182840,-0.01796427922533, 0.222203394789610,-1.63276420106606,-0.000879536547534000,0.000289786686597000,-0.00152657654515900,7.57676958900000e-06,
			2.50949247047356,-7.63747286807608, 0.135993607969712,-1.78183841176321,-0.00137268686404400,1.77069846630000e-05,-0.000697005569516000,5.46586150500000e-06,
			3.55551733749826,-8.75514006657530, 0.231577891465743,-1.47269109306373,-0.00164727375900500,1.94472878550000e-05,-0.00156992282919200,8.74809600300000e-06,
			3.41074572519436,-18.2325539763006, 0.149914391789427,-1.68423249556659,-0.00106915194172600,1.16209240120000e-05,-0.000589221457350000,5.61836654000000e-06,
			3.32879933678709,-21.2950551063640, 0.0985334723578920,-1.43374632887761,-0.00143242470635600,1.13520683510000e-05,-0.000851643213731000,6.20363264600000e-06,
			4.33735589053607,-26.5116664058213, 0.144273978542138,-1.58010535358386,-0.00109609031386100,1.05761414390000e-05,-0.000816717344487000,6.80347557400000e-06
	};


  snr=10*std::log10(snr);
  if (mode.GetModulationClass () == WIFI_MOD_CLASS_ERP_OFDM
      || mode.GetModulationClass () == WIFI_MOD_CLASS_OFDM
      || mode.GetModulationClass () == WIFI_MOD_CLASS_HT
      || mode.GetModulationClass () == WIFI_MOD_CLASS_VHT)
    {
	  double c_1;
	  double c_2;
	  double c_3;
	  double c_4;
	  double d_1;
	  double d_2;
	  double d_3;
	  double d_4;
      if (mode.GetConstellationSize () == 2)
      {
          if (mode.GetCodeRate () == WIFI_CODE_RATE_1_2)
          {
        	  //(3Mbps)
             // if (txVector.GetChannelWidth ()==20)
             // {
            	  pt=&arreglo_modelo[0][0];
               	  c_1 = *pt;
               	  c_2 = *(pt+1);
               	  c_3 = *(pt+2);
               	  c_4 =	*(pt+3);
               	  d_1 = *(pt+4);
               	  d_2 =	*(pt+5);
               	  d_3 = *(pt+6);
               	  d_4 = *(pt+7);

            	  double a_R=c_1*exp(d_1*nbits)+c_2*exp(d_2*nbits);
            	  double b_R=c_3*exp(d_3*nbits)+c_4*exp(d_4*nbits);
            	  double PER_paper=(1-tanh(a_R-b_R*(snr)))/2;
            	  double PER_paper_log = -10*log10(PER_paper); //aded by juan

            	  if (PER_paper_log>10)
            	              	  {
					  pt=&arreglo_modelo1[0][0];
					  c_1 = *pt;
					  c_2 = *(pt+1);
					  c_3 = *(pt+2);
					  c_4 =	*(pt+3);
					  d_1 = *(pt+4);
					  d_2 =	*(pt+5);
					  d_3 = *(pt+6);
					  d_4 = *(pt+7);

					  a_R=c_1*exp(d_1*nbits)+c_2*exp(d_2*nbits);
					  b_R=c_3*exp(d_3*nbits)+c_4*exp(d_4*nbits);
					  PER_paper=(1-tanh(a_R-b_R*(snr)))/2;
				  }

            	  double Su=1-PER_paper;
            	  return Su;
                }
              //else {}
              //}
          else
          {
        	  //4.5Mbps
              //if (txVector.GetChannelWidth ()==20)
              //{
            	  pt=&arreglo_modelo[1][0];
               	  c_1 = *pt;
               	  c_2 = *(pt+1);
               	  c_3 = *(pt+2);
               	  c_4 =	*(pt+3);
               	  d_1 = *(pt+4);
               	  d_2 =	*(pt+5);
               	  d_3 = *(pt+6);
               	  d_4 = *(pt+7);

            	  double a_R=c_1*exp(d_1*nbits)+c_2*exp(d_2*nbits);
            	  double b_R=c_3*exp(d_3*nbits)+c_4*exp(d_4*nbits);
            	  double PER_paper=(1-tanh(a_R-b_R*(snr)))/2;
            	  double PER_paper_log = -10*log10(PER_paper); //added by juan
            	  if (PER_paper_log>10)
				  {
					  pt=&arreglo_modelo1[1][0];
					  c_1 = *pt;
					  c_2 = *(pt+1);
					  c_3 = *(pt+2);
					  c_4 =	*(pt+3);
					  d_1 = *(pt+4);
					  d_2 =	*(pt+5);
					  d_3 = *(pt+6);
					  d_4 = *(pt+7);

					  a_R=c_1*exp(d_1*nbits)+c_2*exp(d_2*nbits);
					  b_R=c_3*exp(d_3*nbits)+c_4*exp(d_4*nbits);
					  PER_paper=(1-tanh(a_R-b_R*(snr)))/2;
				  }

            	  double Su=1-PER_paper;
            	  return Su;
             //   }
             // else {}
          }
      }
      else if (mode.GetConstellationSize () == 4)
      {
    	  if (mode.GetCodeRate () == WIFI_CODE_RATE_1_2)
          {
    		  //6Mbps
             // if (txVector.GetChannelWidth ()==20)
             // {
            	  pt=&arreglo_modelo[2][0];
               	  c_1 = *pt;
               	  c_2 = *(pt+1);
               	  c_3 = *(pt+2);
               	  c_4 =	*(pt+3);
               	  d_1 = *(pt+4);
               	  d_2 =	*(pt+5);
               	  d_3 = *(pt+6);
               	  d_4 = *(pt+7);

            	  double a_R=c_1*exp(d_1*nbits)+c_2*exp(d_2*nbits);
            	  double b_R=c_3*exp(d_3*nbits)+c_4*exp(d_4*nbits);
            	  double PER_paper=(1-tanh(a_R-b_R*(snr)))/2;
            	  double PER_paper_log = -10*log10(PER_paper); //added by juan
            	  if (PER_paper_log>10)
				  {
					  pt=&arreglo_modelo1[2][0];
					  c_1 = *pt;
					  c_2 = *(pt+1);
					  c_3 = *(pt+2);
					  c_4 =	*(pt+3);
					  d_1 = *(pt+4);
					  d_2 =	*(pt+5);
					  d_3 = *(pt+6);
					  d_4 = *(pt+7);

					  a_R=c_1*exp(d_1*nbits)+c_2*exp(d_2*nbits);
					  b_R=c_3*exp(d_3*nbits)+c_4*exp(d_4*nbits);
					  PER_paper=(1-tanh(a_R-b_R*(snr)))/2;
				  }
				  double Su=1-PER_paper;
            	  return Su;
              //}
             // else {}
          }
          else
          {
        	  //9Mbps
            //  if (txVector.GetChannelWidth ()==20)
             // {
            	  pt=&arreglo_modelo[3][0];
               	  c_1 = *pt;
               	  c_2 = *(pt+1);
               	  c_3 = *(pt+2);
               	  c_4 =	*(pt+3);
               	  d_1 = *(pt+4);
               	  d_2 =	*(pt+5);
               	  d_3 = *(pt+6);
               	  d_4 = *(pt+7);

            	  double a_R=c_1*exp(d_1*nbits)+c_2*exp(d_2*nbits);
            	  double b_R=c_3*exp(d_3*nbits)+c_4*exp(d_4*nbits);
            	  double PER_paper=(1-tanh(a_R-b_R*(snr)))/2;
            	  double PER_paper_log = -10*log10(PER_paper);
				  if (PER_paper_log>10)
				  {
					  pt=&arreglo_modelo1[3][0];
					  c_1 = *pt;
					  c_2 = *(pt+1);
					  c_3 = *(pt+2);
					  c_4 =	*(pt+3);
					  d_1 = *(pt+4);
					  d_2 =	*(pt+5);
					  d_3 = *(pt+6);
					  d_4 = *(pt+7);

					  a_R=c_1*exp(d_1*nbits)+c_2*exp(d_2*nbits);
					  b_R=c_3*exp(d_3*nbits)+c_4*exp(d_4*nbits);
					  PER_paper=(1-tanh(a_R-b_R*(snr)))/2;
				  }
				  double Su=1-PER_paper;
            	  return Su;
             // }
             // else {}
          }
      }
      else if (mode.GetConstellationSize () == 16)
      {
    	  if (mode.GetCodeRate () == WIFI_CODE_RATE_1_2)
          {
    		  //12Mbps
             // if (txVector.GetChannelWidth ()==20)
              //{
            	  pt=&arreglo_modelo[4][0];
               	  c_1 = *pt;
               	  c_2 = *(pt+1);
               	  c_3 = *(pt+2);
               	  c_4 =	*(pt+3);
               	  d_1 = *(pt+4);
               	  d_2 =	*(pt+5);
               	  d_3 = *(pt+6);
               	  d_4 = *(pt+7);

               	  double a_R=c_1*exp(d_1*nbits)+c_2*exp(d_2*nbits);
               	  double b_R=c_3*exp(d_3*nbits)+c_4*exp(d_4*nbits);
               	  double PER_paper=(1-tanh(a_R-b_R*(snr)))/2;
			      double PER_paper_log = -10*log10(PER_paper);
			      if (PER_paper_log>10)
				  {
					  pt=&arreglo_modelo1[4][0];
					  c_1 = *pt;
					  c_2 = *(pt+1);
					  c_3 = *(pt+2);
					  c_4 =	*(pt+3);
					  d_1 = *(pt+4);
					  d_2 =	*(pt+5);
					  d_3 = *(pt+6);
					  d_4 = *(pt+7);

					  a_R=c_1*exp(d_1*nbits)+c_2*exp(d_2*nbits);
					  b_R=c_3*exp(d_3*nbits)+c_4*exp(d_4*nbits);
					  PER_paper=(1-tanh(a_R-b_R*(snr)))/2;
				  }
				  double Su=1-PER_paper;
               	  return Su;
             // }
              //else {}
          }
          else
          {
        	  //18Mbps
              //if (txVector.GetChannelWidth ()==20)
             // {
            	  pt=&arreglo_modelo[5][0];
               	  c_1 = *pt;
               	  c_2 = *(pt+1);
               	  c_3 = *(pt+2);
               	  c_4 =	*(pt+3);
               	  d_1 = *(pt+4);
               	  d_2 =	*(pt+5);
               	  d_3 = *(pt+6);
               	  d_4 = *(pt+7);

               	  double a_R=c_1*exp(d_1*nbits)+c_2*exp(d_2*nbits);
               	  double b_R=c_3*exp(d_3*nbits)+c_4*exp(d_4*nbits);
               	  double PER_paper=(1-tanh(a_R-b_R*(snr)))/2;
               	  double PER_paper_log = -10*log10(PER_paper);
				  if (PER_paper_log>10)
				  {
					  pt=&arreglo_modelo1[5][0];
					  c_1 = *pt;
					  c_2 = *(pt+1);
					  c_3 = *(pt+2);
					  c_4 =	*(pt+3);
					  d_1 = *(pt+4);
					  d_2 =	*(pt+5);
					  d_3 = *(pt+6);
					  d_4 = *(pt+7);

					  a_R=c_1*exp(d_1*nbits)+c_2*exp(d_2*nbits);
					  b_R=c_3*exp(d_3*nbits)+c_4*exp(d_4*nbits);
					  PER_paper=(1-tanh(a_R-b_R*(snr)))/2;
				  }
				  double Su=1-PER_paper;
               	  return Su;
              //}
             // else {}
          }
      }
      else if (mode.GetConstellationSize () == 64)
      {
    	  if (mode.GetCodeRate () == WIFI_CODE_RATE_2_3)
          {
    		  //24Mbps
             // if (txVector.GetChannelWidth ()==20)
             // {
            	  pt=&arreglo_modelo[6][0];
               	  c_1 = *pt;
               	  c_2 = *(pt+1);
               	  c_3 = *(pt+2);
               	  c_4 =	*(pt+3);
               	  d_1 = *(pt+4);
               	  d_2 =	*(pt+5);
               	  d_3 = *(pt+6);
               	  d_4 = *(pt+7);

               	  double a_R=c_1*exp(d_1*nbits)+c_2*exp(d_2*nbits);
               	  double b_R=c_3*exp(d_3*nbits)+c_4*exp(d_4*nbits);
               	  double PER_paper=(1-tanh(a_R-b_R*(snr)))/2;
               	  double PER_paper_log = -10*log10(PER_paper);
				  if (PER_paper_log>10)
				  {
					  pt=&arreglo_modelo1[6][0];
					  c_1 = *pt;
					  c_2 = *(pt+1);
					  c_3 = *(pt+2);
					  c_4 =	*(pt+3);
					  d_1 = *(pt+4);
					  d_2 =	*(pt+5);
					  d_3 = *(pt+6);
					  d_4 = *(pt+7);

					  a_R=c_1*exp(d_1*nbits)+c_2*exp(d_2*nbits);
					  b_R=c_3*exp(d_3*nbits)+c_4*exp(d_4*nbits);
					  PER_paper=(1-tanh(a_R-b_R*(snr)))/2;
				  }
				  double Su=1-PER_paper;
               	  return Su;
             // }
             // else {}
          }
          else
          {
        	  //27Mbps
            // if (txVector.GetChannelWidth ()==20)
            // {
           	  pt=&arreglo_modelo[7][0];
              	  c_1 = *pt;
              	  c_2 = *(pt+1);
              	  c_3 = *(pt+2);
              	  c_4 =	*(pt+3);
              	  d_1 = *(pt+4);
              	  d_2 =	*(pt+5);
              	  d_3 = *(pt+6);
              	  d_4 = *(pt+7);

            	 double a_R=c_1*exp(d_1*nbits)+c_2*exp(d_2*nbits);
            	 double b_R=c_3*exp(d_3*nbits)+c_4*exp(d_4*nbits);
            	 double PER_paper=(1-tanh(a_R-b_R*(snr)))/2;

            	 double PER_paper_log = -10*log10(PER_paper);
				if (PER_paper_log>10)
				{
				  pt=&arreglo_modelo1[7][0];
				  c_1 = *pt;
				  c_2 = *(pt+1);
				  c_3 = *(pt+2);
				  c_4 =	*(pt+3);
				  d_1 = *(pt+4);
				  d_2 =	*(pt+5);
				  d_3 = *(pt+6);
				  d_4 = *(pt+7);

				  a_R=c_1*exp(d_1*nbits)+c_2*exp(d_2*nbits);
				  b_R=c_3*exp(d_3*nbits)+c_4*exp(d_4*nbits);
				  PER_paper=(1-tanh(a_R-b_R*(snr)))/2;
				}
				 double Su=1-PER_paper;
            	 return Su;
            // }
            // else {}
          }
      }
    }
  else if (mode.GetModulationClass () == WIFI_MOD_CLASS_DSSS || mode.GetModulationClass () == WIFI_MOD_CLASS_HR_DSSS)
  {
	  switch (mode.GetDataRate (20, 0, 1))
      {
	  case 1000000:
          return DsssErrorRateModel::GetDsssDbpskSuccessRate (snr, nbits);
      case 2000000:
          return DsssErrorRateModel::GetDsssDqpskSuccessRate (snr, nbits);
      case 5500000:
          return DsssErrorRateModel::GetDsssDqpskCck5_5SuccessRate (snr, nbits);
      case 11000000:
          return DsssErrorRateModel::GetDsssDqpskCck11SuccessRate (snr, nbits);
      default:
          NS_ASSERT ("undefined DSSS/HR-DSSS datarate");
      }
  }
  return 0;
}

} //namespace ns3
