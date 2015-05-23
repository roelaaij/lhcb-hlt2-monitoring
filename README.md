# lhcb-hlt2-monitoring
Rate monitoring for LHCb HLT2

The output rates of the LHCb HLT2 must be monitored. Since the HLT2 processes run asynchronously on whatever files are
available on a given local node, and ready for processing, many runs will be processed at the same time across the farm.

The current monitoring infrasctructure is unable to deal with the asynchronous nature of HLT2, so something new is 
needed to ship the histograms created by the tasks and add them up. The ZeroMQ library is being investigated as the
network solution.
