// Communication variables file

// Motor driver command communications
extern shared_data<bool>* commForward;
extern shared_data<bool>* commBackward;
extern shared_data<bool>* commLeft;
extern shared_data<bool>* commRight;

// Navigation data share
extern shared_data<uint16_t>* pos_X; // Global X
extern shared_data<uint16_t>* pos_Y; // Global Y
extern shared_data<uint16_t>* pos_T; // Global Theta (Heading), measured positive CCW from the X axis
									 // Origin is in the center of the ring