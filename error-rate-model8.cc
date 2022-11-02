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
#include "error-rate-model8.h"
#include "wifi-phy.h"
#include "ns3/log.h"


namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("ErrorRateModel8");

NS_OBJECT_ENSURE_REGISTERED (ErrorRateModel8);

TypeId
ErrorRateModel8::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ErrorRateModel8")
    .SetParent<ErrorRateModel> ()
    .SetGroupName ("Wifi")
    .AddConstructor<ErrorRateModel8> ()
  ;
  return tid;
}

ErrorRateModel8::ErrorRateModel8 ()

//Begin added by juan
: modelType (15)

//end added by juan
{
}


//==begin added by juan

void
ErrorRateModel8::SetErrorModel(uint32_t _model)
{
  modelType = _model;
}
uint32_t
ErrorRateModel8::GetModel (void) const
{
  return modelType;
}

//===addded by juan


double
ErrorRateModel8::GetBpskBer (double snr) const
{
  NS_LOG_FUNCTION (this << snr);
  double z = std::sqrt (snr);
  double ber = 0.5 * erfc (z);
  NS_LOG_INFO ("bpsk snr=" << snr << " ber=" << ber);
  return ber;
}

double
ErrorRateModel8::GetQpskBer (double snr) const
{
  NS_LOG_FUNCTION (this << snr);
  double z = std::sqrt (snr / 2.0);
  double ber = 0.5 * erfc (z);
  NS_LOG_INFO ("qpsk snr=" << snr << " ber=" << ber);
  return ber;
}

double
ErrorRateModel8::Get16QamBer (double snr) const
{
  NS_LOG_FUNCTION (this << snr);
  double z = std::sqrt (snr / (5.0 * 2.0));
  double ber = 0.75 * 0.5 * erfc (z);
  NS_LOG_INFO ("16-Qam" << " snr=" << snr << " ber=" << ber);
  return ber;
}

double
ErrorRateModel8::Get64QamBer (double snr) const
{
  NS_LOG_FUNCTION (this << snr);
  double z = std::sqrt (snr / (21.0 * 2.0));
  double ber = 7.0 / 12.0 * 0.5 * erfc (z);
  NS_LOG_INFO ("64-Qam" << " snr=" << snr << " ber=" << ber);
  return ber;
}
double
ErrorRateModel8::Get256QamBer (double snr) const
{
  NS_LOG_FUNCTION (this << snr);
  double z = std::sqrt (snr / (85.0 * 2.0));
  double ber = 15.0 / 32.0 * 0.5 * erfc (z);
  NS_LOG_INFO ("256-Qam" << " snr=" << snr << " ber=" << ber);
  return ber;
}

double
ErrorRateModel8::GetFecBpskBer (double snr, uint32_t nbits,
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
ErrorRateModel8::GetFecQpskBer (double snr, uint32_t nbits,
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
ErrorRateModel8::CalculatePe (double p, uint32_t bValue) const
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
ErrorRateModel8::GetFec16QamBer (double snr, uint32_t nbits,
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
ErrorRateModel8::GetFec64QamBer (double snr, uint32_t nbits,
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
ErrorRateModel8::GetFec256QamBer (double snr, uint32_t nbits,
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
ErrorRateModel8::GetChunkSuccessRate (WifiMode mode, WifiTxVector txVector, double snr, uint32_t nbits) const
{


	//std::cout << GetModel() <<"\n";

  double *pt;

	double arreglo_modelo[8][8] = {
			3.64586491152694,-3.44092737951818, 0.507662641851073, -0.962851433845432,-0.00135748494916600,4.11659530700000e-05,-0.00202341705980600,2.20317557640000e-05,
			4.48171979822115,-5.07149268437161, 0.504080220574546,-0.958035443905520,-0.00150873848073600,3.61740920950000e-05,-0.00206161039756300,2.21918607150000e-05,
			5.12101505758392,-6.30538338103856, 0.505590327590540,-0.962911473143776,-0.00151743878136500,3.31635743060000e-05,-0.00197594446622600,2.17654166720000e-05,
			5.78145674973695,-7.68659192663585, 0.485532921368855,-0.933291069533266,-0.00177290110685800,3.45057142310000e-05,-0.00225982973464700,2.45681610620000e-05,
			7.93965445569684,-11.6927779990548, 0.471836365159739,-0.873224921515755,-0.00185213754604900,3.26502232590000e-05,-0.00247253542625000,2.71369025080000e-05,
			9.00653678542744,-13.3861445556868, 0.484293710707878,-0.892584114046291,-0.00191003958326000,3.06830522700000e-05,-0.00220458742160800,2.47367036110000e-05,
			10.7128450446588,-16.7974263714224, 0.439757838410512,-0.820202625556524,-0.00205202450638900,3.52768419730000e-05,-0.00231027190033000,3.04598525450000e-05,
			10.6172673883474,-16.2922205234529, 0.431478192477952,-0.778889437744041,-0.00276659419511200,4.09127425260000e-05,-0.00329294707230100,3.58993099290000e-05
	};

	//(10 - en adelante)

	double arreglo_modelo1[8][8] = {
			4.14234585717076,-4.86536918335464, 0.421380251589724, -1.26795871201802,-0.00155598218133600,3.16173923570000e-05,-0.00172915405155700,1.32711265170000e-05,
			5.11064582667763,-7.15021306122026, 0.460168017515940,-1.27717769386733,-0.00165442109457300,2.55637647700000e-05,-0.00185179600017600,1.25895510670000e-05,
			5.32272552300413,-8.55378640253555, 0.414366836669799,-1.25689285349013,-0.00166357218841100,2.53393046080000e-05,-0.00180313148100600,1.40308370120000e-05,
			6.43294470512147,-10.9097853664419, 0.455713244865281,-1.27111917480121,-0.00171091821350300,2.19350440680000e-05,-0.00185277454571000,1.30029674060000e-05,
			7.97881087107872,-16.0185837945465, 0.408086134395531,-1.18136491199331,-0.00209329056430600,2.48969185470000e-05,-0.00241274977923600,1.84038521760000e-05,
			8.61169194876993,-18.1329215316931, 0.402528332859269,-1.18325149393763,-0.00208782638674300,2.36771150090000e-05,-0.00237792721752200,1.79702194620000e-05,
			11.0063566801119,-24.0241274023891, 0.409665934722813,-1.15256086935968,-0.00234011598015300,2.35132061630000e-05,-0.00257961655945100,1.89265061750000e-05,
			11.0568786502572,-24.5633891313650, 0.403005960749043,-1.15054342547446,-0.00215983634271300,2.34343231620000e-05,-0.00234411291352700,1.89405047020000e-05
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
