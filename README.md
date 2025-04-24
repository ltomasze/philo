<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>philo - README</title>
</head>
<body>
  <h1>philo</h1>

  <p><strong>philo</strong> is a project from the 42 curriculum that implements a solution to the Dining Philosophers problem using threads and mutexes in C.<br>
  The goal is to simulate the behavior of philosophers who alternate between thinking, eating, and sleeping, while managing shared resources (forks) without causing deadlocks or data races.</p>

  <p>Each philosopher is represented by a thread.<br>
  To eat, a philosopher must pick up two forks (mutexes), one on the left and one on the right.<br>
  The program ensures that all actions (eating, thinking, sleeping, and dying) are synchronized and that output is printed without overlapping or corruption.<br>
  The program must also detect and properly handle the case where a philosopher dies if they do not eat within a given time limit.</p>

  <p>The program is compiled using <code>make</code>, and can be run with the following arguments:</p>

  <pre><code>
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
  </code></pre>

  <ul>
    <li><code>number_of_philosophers</code>: the number of philosophers (and forks)</li>
    <li><code>time_to_die</code>: time in milliseconds a philosopher can live without eating</li>
    <li><code>time_to_eat</code>: time in milliseconds a philosopher takes to eat</li>
    <li><code>time_to_sleep</code>: time in milliseconds a philosopher takes to sleep</li>
    <li><code>number_of_times_each_philosopher_must_eat</code> (optional): if provided, simulation stops once every philosopher has eaten at least this many times</li>
  </ul>

  <p>Example:</p>

  <pre><code>
./philo 5 800 200 200
  </code></pre>

  <p>This command runs the simulation with 5 philosophers who die if they don’t eat within 800ms, take 200ms to eat, and 200ms to sleep.</p>

  <p>The implementation must not produce data races or deadlocks and should precisely handle timing using <code>gettimeofday</code> and custom sleep functions instead of <code>usleep</code>.<br>
  Each philosopher must behave independently while respecting resource constraints and shared access to forks and output.</p>
</body>
</html>
