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
#include "error-rate-model7.h"
#include "wifi-phy.h"
#include "ns3/log.h"


namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("ErrorRateModel7");

NS_OBJECT_ENSURE_REGISTERED (ErrorRateModel7);

TypeId
ErrorRateModel7::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ErrorRateModel7")
    .SetParent<ErrorRateModel> ()
    .SetGroupName ("Wifi")
    .AddConstructor<ErrorRateModel7> ()
  ;
  return tid;
}

ErrorRateModel7::ErrorRateModel7 ()

//Begin added by juan
: modelType (15)

//end added by juan
{
}


//==begin added by juan

void
ErrorRateModel7::SetErrorModel(uint32_t _model)
{
  modelType = _model;
}
uint32_t
ErrorRateModel7::GetModel (void) const
{
  return modelType;
}

//===addded by juan


double
ErrorRateModel7::GetBpskBer (double snr) const
{
  NS_LOG_FUNCTION (this << snr);
  double z = std::sqrt (snr);
  double ber = 0.5 * erfc (z);
  NS_LOG_INFO ("bpsk snr=" << snr << " ber=" << ber);
  return ber;
}

double
ErrorRateModel7::GetQpskBer (double snr) const
{
  NS_LOG_FUNCTION (this << snr);
  double z = std::sqrt (snr / 2.0);
  double ber = 0.5 * erfc (z);
  NS_LOG_INFO ("qpsk snr=" << snr << " ber=" << ber);
  return ber;
}

double
ErrorRateModel7::Get16QamBer (double snr) const
{
  NS_LOG_FUNCTION (this << snr);
  double z = std::sqrt (snr / (5.0 * 2.0));
  double ber = 0.75 * 0.5 * erfc (z);
  NS_LOG_INFO ("16-Qam" << " snr=" << snr << " ber=" << ber);
  return ber;
}

double
ErrorRateModel7::Get64QamBer (double snr) const
{
  NS_LOG_FUNCTION (this << snr);
  double z = std::sqrt (snr / (21.0 * 2.0));
  double ber = 7.0 / 12.0 * 0.5 * erfc (z);
  NS_LOG_INFO ("64-Qam" << " snr=" << snr << " ber=" << ber);
  return ber;
}
double
ErrorRateModel7::Get256QamBer (double snr) const
{
  NS_LOG_FUNCTION (this << snr);
  double z = std::sqrt (snr / (85.0 * 2.0));
  double ber = 15.0 / 32.0 * 0.5 * erfc (z);
  NS_LOG_INFO ("256-Qam" << " snr=" << snr << " ber=" << ber);
  return ber;
}

double
ErrorRateModel7::GetFecBpskBer (double snr, uint32_t nbits,
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
ErrorRateModel7::GetFecQpskBer (double snr, uint32_t nbits,
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
ErrorRateModel7::CalculatePe (double p, uint32_t bValue) const
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
ErrorRateModel7::GetFec16QamBer (double snr, uint32_t nbits,
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
ErrorRateModel7::GetFec64QamBer (double snr, uint32_t nbits,
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
ErrorRateModel7::GetFec256QamBer (double snr, uint32_t nbits,
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
ErrorRateModel7::GetChunkSuccessRate (WifiMode mode, WifiTxVector txVector, double snr, uint32_t nbits) const
{


	//std::cout << GetModel() <<"\n";

  double *pt;

	double arreglo_modelo[8][8] = {
			3.18712005311264,-2.53402985982601, 0.512124344709710, -0.967172461695009,-0.00127065833947700,4.64052888920000e-05,-0.00199936292221500,2.15894126840000e-05,
			4.01704107198649,-4.17524473538837, 0.504976602094147,-0.960075820074446,-0.00142321602435100,3.85836303400000e-05,-0.00203928598415800,2.22624944680000e-05,
			4.65038115371709,-5.37439284893574, 0.509346078667877,-0.963164266385265,-0.00150242974481100,3.51972873420000e-05,-0.00203467039200700,2.19546354410000e-05,
			5.47730184715388,-7.01554037125649, 0.501705177011373,-0.957141467753506,-0.00157840283168600,3.29193538490000e-05,-0.00203886867392800,2.25430338380000e-05,
			7.53217053924909,-10.7199295096673, 0.475893528064206,-0.875051759871723,-0.00206147512547500,3.70289195970000e-05,-0.00255818529625800,2.95758262450000e-05,
			8.52358249699360,-12.8819941683319, 0.476747044319774,-0.909500805747538,-0.00150797467714500,2.68734807300000e-05,-0.00175072940328900,2.15145123820000e-05,
			10.8800376302248,-16.2502253423559, 0.472024439756865,-0.833005772269576,-0.00223555302116900,3.43987779230000e-05,-0.00256339994311000,2.93489797700000e-05,
			10.3900143773572,-15.9502061272675, 0.435539139906829,-0.802675513368845,-0.00261996927367500,3.65854207360000e-05,-0.00294023880374300,3.09399349870000e-05
	};

	//(10 - en adelante)

	double arreglo_modelo1[8][8] = {
			3.71357779681352,-3.60148816807538, 0.416307879146935, -1.25632009312102,-0.00153470547759200,3.77990896950000e-05,-0.00176811900491000,1.40504360840000e-05,
			4.52269300582032,-5.82749587885524, 0.436217417932856,-1.26074150498730,-0.00166660082153000,2.93279382790000e-05,-0.00197891138142400,1.37726980680000e-05,
			4.91030190560169,-7.32387712830015, 0.412802735092729,-1.25354393953141,-0.00159383505250600,2.71758590830000e-05,-0.00172679398405800,1.42402589740000e-05,
			5.67764015094186,-9.60633279394574, 0.416897031670484,-1.26270677333351,-0.00163723951892500,2.36418279620000e-05,-0.00175596141740800,1.35796168100000e-05,
			7.71959109498556,-15.4414590996220, 0.413714542898705,-1.22089631163770,-0.00160202357818000,2.13489390340000e-05,-0.00168196694213600,1.48401799680000e-05,
			8.30157351114409,-16.9489236808397, 0.407761124115450,-1.18249859502912,-0.00213110872577500,2.39599777400000e-05,-0.00240565699195100,1.77524478240000e-05,
			10.3945334477579,-23.3870872693267, 0.397010232996495,-1.17170738525975,-0.00176504636023400,2.20401828600000e-05,-0.00189255929120100,1.76280121170000e-05,
			10.5106560357283,-23.3625099988628, 0.393815868483560,-1.14845210184909,-0.00218974291529700,2.40405833790000e-05,-0.00231488009942000,1.91387094500000e-05
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
