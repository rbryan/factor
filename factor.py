from fractions import gcd
import random
import time
import pickle
import socket

HOST = "127.0.0.1";
PORT = 50000;

num = 5691972411341955564803;
randfactor = 0;

start_time = time.time();


def f(n):
	return (n*n+randfactor)%num;

def prho():
	global randfactor;
	random.seed(time.time());

	while True:
		randfactor = random.randint(0,num);
		a = random.randint(0,num);
		b = a;
		p=1;

		while True:
			a = f(a);
			b = f(f(b));
			p = gcd(abs(b-a),num);
			if(p > 1):
				break;
			if(b==a):
				break;

		if not p==1 and not p==num:
			print p;
			send_num(p);
			print num/p;
			
			break;

def send_num(n):
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM);
	s.connect((HOST,PORT));
	data = pickle.dumps((n,num/n,time.time()-start_time));
	s.send(data);
	s.close;

prho();
