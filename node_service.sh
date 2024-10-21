
#!/bin/bash

# Function to start node monitoring
start_node_monitoring() {
  echo "Starting node monitoring service..."
  nohup ./monitoring_node > node_output.log 2>&1 &
  echo "Node monitoring started."
}

# Function to stop node monitoring
stop_node_monitoring() {
  echo "Stopping node monitoring service..."
  pkill -f monitoring_node
  echo "Node monitoring stopped."
}

case $1 in
  start)
    start_node_monitoring
    ;;
  stop)
    stop_node_monitoring
    ;;
  *)
    echo "Usage: $0 {start|stop}"
    ;;
esac
