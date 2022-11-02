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
#include "error-rate-model3.h"
#include "wifi-phy.h"
#include "ns3/log.h"


namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("ErrorRateModel3");

NS_OBJECT_ENSURE_REGISTERED (ErrorRateModel3);

TypeId
ErrorRateModel3::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ErrorRateModel3")
    .SetParent<ErrorRateModel> ()
    .SetGroupName ("Wifi")
    .AddConstructor<ErrorRateModel3> ()
  ;
  return tid;
}

ErrorRateModel3::ErrorRateModel3 ()

//Begin added by juan
: modelType (15)

//end added by juan
{
}


//==begin added by juan

void
ErrorRateModel3::SetErrorModel(uint32_t _model)
{
  modelType = _model;
}
uint32_t
ErrorRateModel3::GetModel (void) const
{
  return modelType;
}

//===addded by juan


double
ErrorRateModel3::GetBpskBer (double snr) const
{
  NS_LOG_FUNCTION (this << snr);
  double z = std::sqrt (snr);
  double ber = 0.5 * erfc (z);
  NS_LOG_INFO ("bpsk snr=" << snr << " ber=" << ber);
  return ber;
}

double
ErrorRateModel3::GetQpskBer (double snr) const
{
  NS_LOG_FUNCTION (this << snr);
  double z = std::sqrt (snr / 2.0);
  double ber = 0.5 * erfc (z);
  NS_LOG_INFO ("qpsk snr=" << snr << " ber=" << ber);
  return ber;
}

double
ErrorRateModel3::Get16QamBer (double snr) const
{
  NS_LOG_FUNCTION (this << snr);
  double z = std::sqrt (snr / (5.0 * 2.0));
  double ber = 0.75 * 0.5 * erfc (z);
  NS_LOG_INFO ("16-Qam" << " snr=" << snr << " ber=" << ber);
  return ber;
}

double
ErrorRateModel3::Get64QamBer (double snr) const
{
  NS_LOG_FUNCTION (this << snr);
  double z = std::sqrt (snr / (21.0 * 2.0));
  double ber = 7.0 / 12.0 * 0.5 * erfc (z);
  NS_LOG_INFO ("64-Qam" << " snr=" << snr << " ber=" << ber);
  return ber;
}
double
ErrorRateModel3::Get256QamBer (double snr) const
{
  NS_LOG_FUNCTION (this << snr);
  double z = std::sqrt (snr / (85.0 * 2.0));
  double ber = 15.0 / 32.0 * 0.5 * erfc (z);
  NS_LOG_INFO ("256-Qam" << " snr=" << snr << " ber=" << ber);
  return ber;
}

double
ErrorRateModel3::GetFecBpskBer (double snr, uint32_t nbits,
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
ErrorRateModel3::GetFecQpskBer (double snr, uint32_t nbits,
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
ErrorRateModel3::CalculatePe (double p, uint32_t bValue) const
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
ErrorRateModel3::GetFec16QamBer (double snr, uint32_t nbits,
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
ErrorRateModel3::GetFec64QamBer (double snr, uint32_t nbits,
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
ErrorRateModel3::GetFec256QamBer (double snr, uint32_t nbits,
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
ErrorRateModel3::GetChunkSuccessRate (WifiMode mode, WifiTxVector txVector, double snr, uint32_t nbits) const
{


	//std::cout << GetModel() <<"\n";

  double *pt;

	double arreglo_modelo[8][8] = {
			2.95273153815820,-5.49937095816888, 0.380191424504812, -1.62824884013951,-0.000880615884263000,2.33061382420000e-05,-0.00111366968978900,1.16273869190000e-05,
			3.09728164355334,-6.80421746853342, 0.328041236715921,-1.53349089854729,-0.00107506745469100,2.62460040570000e-05,-0.00141170994740300,1.57465105520000e-05,
			2.94555359381293,-5.49129502697386, 0.378488701301435,-1.62612532456776,-0.000884430380729000,2.35667682390000e-05,-0.00112609273185800,1.18648439700000e-05,
			3.18602993951641,-6.91428039727763, 0.340445596363895,-1.55023742499814,-0.000998639096183000,2.42517148510000e-05,-0.00125592888617500,1.42831896730000e-05,
			2.97391700051886,-5.52390591432793, 0.384960438942971,-1.63434376147593,-0.000867822009011000,2.26468069970000e-05,-0.00107669556353200,1.10499534670000e-05,
			3.28558179937902,-7.04744951971556, 0.352595041899999,-1.56874327525276,-0.000899594284047000,2.32498462880000e-05,-0.00107517929067600,1.39311922080000e-05,
			2.59146198280202,-4.09167669789535, 0.244193142408141,-1.26401467214646,-0.000899345247847000,3.17024961030000e-05,-0.00100520781352600,1.59137151110000e-05,
			3.47220152950315,-7.25074924666894, 0.392318485661446,-1.61442331618719,-0.000829729801280000,1.72603548710000e-05,-0.000882757613872000,8.08035128600000e-06
	};

	//(10 - en adelante)

	double arreglo_modelo1[8][8] = {
			3.21273647479387,-6.30990319895334, 0.435776736299653, -1.79355058967980,-0.00102665869510000,2.01007901310000e-05,-0.00164253811735200,9.55161661600000e-06,
			3.68205549159713,-7.97989082820422, 0.444811555882664,-1.73555630536803,-0.00125298595819200,2.02169356510000e-05,-0.00196290894721200,1.08930567590000e-05,
			3.21108136657091,-6.30795651608752, 0.435578904493147,-1.79326121278733,-0.00102777930847800,2.01531113270000e-05,-0.00164518583325000,9.58371661400000e-06,
			3.67988976011791,-7.97694128872967, 0.444660837951036,-1.73527716518577,-0.00125544568351400,2.02521895380000e-05,-0.00196665371902500,1.09077416010000e-05,
			3.21072981996537,-6.30750248691130, 0.435552294935388,-1.79321423266091,-0.00102811687071400,2.01592848150000e-05,-0.00164572725152600,9.58558896800000e-06,
			3.67496238028385,-7.97114333662946, 0.444128908633577,-1.73449943891260,0.00125885462044900,2.04187796460000e-05,-0.00197464977379500,1.10258918250000e-05,
			3.42708506552991,-5.23800508099319, 0.447464627923785,-1.50503031772577,-0.00118268929494700,2.51581033950000e-05,-0.00200706298383900,1.18407627240000e-05,
			3.68305359796586,-7.97981096480704, 0.445179196299169,-1.73590901478081,-0.00125531734525200,2.00651727790000e-05,-0.00196207306209600,1.07534119080000e-05
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
