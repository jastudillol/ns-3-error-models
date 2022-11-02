# ns-3-error-models
This work focuses on improving the realism of the PHY of Network simulators by proposing a technique to obtain a simple model for implementing several theoretical PER models and even simulated PER models into the PHY. These models are implemented in the ns-3 network simulator to improve their realism.

The error models integrated in the simulator to improve the PHY layer realism are as follows:

Model 1: Model for vehicular networks by interpolation
Model 2: Model without coding over an AWGN channel using EVT
Model 3: Model of non-homogeneous errors at PHY layer
Model 4: Model of PER upper bound for OFDM systems, with BER equation #1
Model 5: Model of PER upper bound for OFDM systems, with BER model #2
Model 6: Model of PER upper bound for OFDM systems, with BER model #3
Model 7: Model without coding over an AWGN channel, with BER model #1.
Model 8: Model without coding on AWGN channel, with BER model #2.
Model 9: Model without coding on AWGN channel, with BER model #3.

The use of the models is simple, just place the model files in the "nsXX/src/wifi/model/" directory.

To avoid errors in the compilation of the new error models it is necessary to add the name of the source files ".cc" and headers ".h" in the "wscript" file as follows:

'model/error-rate-model2.cc'

'model/error-rate-model3.cc'

'model/error-rate-model4.cc'

'model/error-rate-model5.cc'

'model/error-rate-model6.cc'

'model/error-rate-model7.cc'

'model/error-rate-model8.cc'

'model/error-rate-model9.cc'

'model/error-rate-model2.h'

'model/error-rate-model3.h'

'model/error-rate-model4.h'

'model/error-rate-model5.h'

'model/error-rate-model6.h'

'model/error-rate-model7.h'

'model/error-rate-model8.h'

'model/error-rate-model9.h'

Finally, the selection of the new error model in the simulation file is done in a simple way, as shown below:

errorModelType = "ns3::ErrorRateModel2";

wifi.SetErrorRateModel (errorModelType);

