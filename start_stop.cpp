#include <iostream>
#include <complex>

#include <uhd/usrp/multi_usrp.hpp>
#include <uhd/utils/msg.hpp>

char dummy_buffer[1024*1024];

int recv(uhd::rx_streamer::sptr rx_stream, int packet_len, int nof_packets) {
  uhd::rx_metadata_t md;
  uint32_t recv_packets = 0; 
  int n;
  do {
    n = rx_stream->recv(dummy_buffer, packet_len, md, 1);
    if (n < 0) {
     return -1; 
    }
    recv_packets++;
  } while (n > 0                                                     && 
           md.error_code == uhd::rx_metadata_t::ERROR_CODE_NONE      && 
           recv_packets < nof_packets);
  return recv_packets;  
}


int start_rx_stream(uhd::usrp::multi_usrp::sptr usrp)
{
  uhd::stream_cmd_t cmd(uhd::stream_cmd_t::STREAM_MODE_START_CONTINUOUS);
//  cmd.time_spec = usrp->get_time_now();
  cmd.stream_now = true;
//  cmd.time_spec += 0.2;
  usrp->issue_stream_cmd(cmd);
  return 0;
}

int stop_rx_stream(uhd::usrp::multi_usrp::sptr usrp)
{
  uhd::stream_cmd_t cmd(uhd::stream_cmd_t::STREAM_MODE_STOP_CONTINUOUS);
  cmd.time_spec = usrp->get_time_now();
  cmd.stream_now = true;
  usrp->issue_stream_cmd(cmd);
  return 0;
}

int main(int argc, char **argv) {
  
  std::string args = std::string("master_clock_rate=30720000");
  uhd::usrp::multi_usrp::sptr usrp = uhd::usrp::multi_usrp::make(args);

  usrp->set_clock_source("internal");

  std::string otw, cpu;
  otw = "sc16";
  cpu = "fc32";
  uhd::stream_args_t stream_args(cpu, otw);
  uhd::rx_streamer::sptr rx_stream = usrp->get_rx_stream(stream_args);

  double srate = 1.92e6;
  double freq1 = 3.0e9;

  std::cout << "Setting sample rate " << srate << std::endl;  
  usrp->set_rx_rate(srate);
  usrp->set_rx_freq(freq1);  
  
  uint32_t nof_restart=0;
  bool error = false; 
  while(!error) {

    start_rx_stream(usrp);
    int n = recv(rx_stream, 1920, 300);
    if (n<0) {
     std::cout << "Error" << std::endl;
     error = true; 
    } else {
     nof_restart++;
    }
    stop_rx_stream(usrp);
//    sleep(1);
    std::cout << "Done " << nof_restart << " restarts (" << n << " packets)" << std::endl;    
  }
  
  exit(0);
}
