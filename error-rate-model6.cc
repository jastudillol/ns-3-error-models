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
#include "error-rate-model6.h"
#include "wifi-phy.h"
#include "ns3/log.h"


namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("ErrorRateModel6");

NS_OBJECT_ENSURE_REGISTERED (ErrorRateModel6);

TypeId
ErrorRateModel6::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ErrorRateModel6")
    .SetParent<ErrorRateModel> ()
    .SetGroupName ("Wifi")
    .AddConstructor<ErrorRateModel6> ()
  ;
  return tid;
}

ErrorRateModel6::ErrorRateModel6 ()

//Begin added by juan
: modelType (15)

//end added by juan
{
}


//==begin added by juan

void
ErrorRateModel6::SetErrorModel(uint32_t _model)
{
  modelType = _model;
}
uint32_t
ErrorRateModel6::GetModel (void) const
{
  return modelType;
}

//===addded by juan


double
ErrorRateModel6::GetBpskBer (double snr) const
{
  NS_LOG_FUNCTION (this << snr);
  double z = std::sqrt (snr);
  double ber = 0.5 * erfc (z);
  NS_LOG_INFO ("bpsk snr=" << snr << " ber=" << ber);
  return ber;
}

double
ErrorRateModel6::GetQpskBer (double snr) const
{
  NS_LOG_FUNCTION (this << snr);
  double z = std::sqrt (snr / 2.0);
  double ber = 0.5 * erfc (z);
  NS_LOG_INFO ("qpsk snr=" << snr << " ber=" << ber);
  return ber;
}

double
ErrorRateModel6::Get16QamBer (double snr) const
{
  NS_LOG_FUNCTION (this << snr);
  double z = std::sqrt (snr / (5.0 * 2.0));
  double ber = 0.75 * 0.5 * erfc (z);
  NS_LOG_INFO ("16-Qam" << " snr=" << snr << " ber=" << ber);
  return ber;
}

double
ErrorRateModel6::Get64QamBer (double snr) const
{
  NS_LOG_FUNCTION (this << snr);
  double z = std::sqrt (snr / (21.0 * 2.0));
  double ber = 7.0 / 12.0 * 0.5 * erfc (z);
  NS_LOG_INFO ("64-Qam" << " snr=" << snr << " ber=" << ber);
  return ber;
}
double
ErrorRateModel6::Get256QamBer (double snr) const
{
  NS_LOG_FUNCTION (this << snr);
  double z = std::sqrt (snr / (85.0 * 2.0));
  double ber = 15.0 / 32.0 * 0.5 * erfc (z);
  NS_LOG_INFO ("256-Qam" << " snr=" << snr << " ber=" << ber);
  return ber;
}

double
ErrorRateModel6::GetFecBpskBer (double snr, uint32_t nbits,
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
ErrorRateModel6::GetFecQpskBer (double snr, uint32_t nbits,
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
ErrorRateModel6::CalculatePe (double p, uint32_t bValue) const
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
ErrorRateModel6::GetFec16QamBer (double snr, uint32_t nbits,
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
ErrorRateModel6::GetFec64QamBer (double snr, uint32_t nbits,
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
ErrorRateModel6::GetFec256QamBer (double snr, uint32_t nbits,
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
ErrorRateModel6::GetChunkSuccessRate (WifiMode mode, WifiTxVector txVector, double snr, uint32_t nbits) const
{


	//std::cout << GetModel() <<"\n";

  double *pt;

	double arreglo_modelo[8][8] = {
			0.335291664744712, 2.23574472295461, 0.526893265712439, -0.828996427669230,-0.00235787512680500,-3.79684645860000e-05,-0.00108985380149900,2.56951745480000e-05,
			3.22627342985238,-7.72707732317285, 0.323505773495795,-2.31395536084290,-0.00224218339467600,2.38738277050000e-05,-0.00438078263616900,1.18870425480000e-05,
			2.23090824068082,-0.736606359532674, 0.555194255579575,-0.810491549672661,-0.000768452876922000,8.11634845590000e-05,-0.00133237863502100,2.78724912440000e-05,
			4.99288993069917,-15.7061537536960, 0.433026253485951,-2.46605809414054,-0.00151869636174200,1.08539604380000e-05,-0.00141021966231900,4.67821801400000e-06,
			8.81904411460908,-15.1685982636890, 0.862025788210656,-1.90788452141029,-0.00141245908409400,1.92115595050000e-05,-0.00152849046933700,1.40234780260000e-05,
			4.07975295700955,-39.8580158614952, 0.193627133829597,-3.42856118172394,-0.00181413133376600,1.14192879260000e-05,-0.000824737433929000,7.84350273500000e-06,
			11.9058287475515,-56.7503038844136, 0.690740356157001,-3.97389297551151,-0.00213000623531800,1.09369391050000e-05,-0.00220936189031600,9.25272456700000e-06,
			5.50256881283438,-72.2461203632284, 0.236404052965108,-4.73297258096957,-0.00304282714148000,8.19396237100000e-06,-0.00439946902232000,6.85187293200000e-06
	};

	//(10 - en adelante)

	double arreglo_modelo1[8][8] = {
			1.28559917013792,2.73862154881481,0.612513362256033, -1.26851961143474,-0.00183303494596700,-5.99433013100000e-05,-0.00180326609582800,2.19663854090000e-05,
			4.47224587170416,-9.03318905901939, 0.615261956164304,-2.59180864699573,-0.00133231288291500,2.22333350610000e-05,-0.00120683245609900,1.16169640610000e-05,
			3.50777452102760,-1.56824274751812, 0.611163530459552,-1.24954783880221,-0.00138108144089000,7.01359090690000e-05,-0.00217398695004000,2.34586472950000e-05,
			6.25760971723200,-16.6754763015406, 0.606273894611279,-2.57138718927113,-0.00145726245782200,1.80355291480000e-05,-0.00141320297359100,1.20671939960000e-05,
			9.09752365956446,-19.1725861010277, 0.811793100557465,-2.36750481680100,-0.00166851829893100,2.08511262260000e-05,-0.00173288334571200,1.54111143970000e-05,
			10.9838847054258,-46.4165323920125, 0.744209291716758,-3.91732373457366,-0.00109796661608800,9.33876194100000e-06,-0.00102355959340300,7.21119546200000e-06,
			13.4471984991730,-62.6097119960087, 0.784588787126040,-4.36857424847745,-0.00158011263515500,5.97019898200000e-06,-0.00155408140195100,4.50390431000000e-06,
			16.9781205762569,-79.8073911369286, 0.963107165952382,-5.21284829834553,-0.00190239357592000,1.01717214540000e-05,-0.00189928186171600,8.85410943500000e-06
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
