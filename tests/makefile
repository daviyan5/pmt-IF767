SHELL := /bin/bash
PMT = ../bin/pmt
PATTERN_FILE = ../tests/texts/patterns.txt
PI_FILE = ../tests/texts/pattern
EXACT_ALG = kmp boyermoore bruteforce
APROX_ALG = sellers wumanber
MULT_ALG = ahocorasick
SHORT_PAT = AuQ
MEDIUM_PAT = MDuzgMWSZcX
LONG_PAT = MAglYtHkGlAUZGKeulyRPaEuRDvExQLwGAkDTBihMfRjijaNxKHRqdlY
DEFAULT_FILE = ../tests/texts/text_0.txt
ANY_FILE = ../tests/texts/text_
ALL_FILES = ../tests/texts/text_*.txt
PY_AVERAGE = ./time_outputs/average_time.py
help:
	@echo "Você pode criar os arquivos de teste com "make config"";
	@echo "Atenção: pelo menos 4GB de dados serão criados.";
	@echo "Para executar os testes, use "make Ti", onde i é o número do teste.";
	@echo "Além das dependências de execução do comando (GCC), para executar os testes também é necessário python."
	@echo "Os resultados de cada teste ficarão na pasta ./results";
config:
	g++ -O3 ./texts/create_texts.cpp -o ./texts/create_texts
	./text/create_texts
T1:
	@for alg in $(EXACT_ALG); do \
		echo "começando testes com algoritmo: "$$alg; \
		for i in 0 1 2 3 4; do \
			echo "Teste $$i"; \
			for j in 1 2 3 4 5 6 7 8 9 10; do \
				COMMAND="$(PMT) -c -a $$alg $(SHORT_PAT) $(ANY_FILE)$$i.txt" ; \
				{ time $$COMMAND ;} 2>./time_outputs/T1_temp_f$$j.txt ; \
			done; \
			echo "done."; \
			python $(PY_AVERAGE) ./time_outputs/T1_temp_f*.txt > ./results/T1_$$alg$$i.txt ; \
			rm ./time_outputs/T1_temp_f*.txt ; \
		done ; \
		echo "Threads em $$alg" ; \
		COMMAND="$(PMT) -c -a $$alg $(SHORT_PAT) $(ALL_FILES)" ; \
		{ time $$COMMAND ;} 2>./time_outputs/T1_threads_$$alg.txt ; \
		python $(PY_AVERAGE) ./time_outputs/T1_threads_*.txt > ./results/T1_threads_$$alg.txt ; \
	done
	for i in 0 1 2 3 4; do \
		echo "Teste grep"; \
		for j in 1 2 3 4 5 6 7 8 9 10; do \
			COMMAND="grep -c $(SHORT_PAT) $(ANY_FILE)$$i.txt" ; \
			{ time $$COMMAND ;} 2>./time_outputs/T1_temp_f$$j.txt ; \
		done; \
		echo "done."; \
		python $(PY_AVERAGE) ./time_outputs/T1_temp_f*.txt > ./results/T1_grep$$i.txt ; \
		rm ./time_outputs/T1_temp_f*.txt ; \
	done ;
	echo "Todos os arquivos com grep" ; \
	COMMAND="grep -c $(SHORT_PAT) $(ALL_FILES)" ; \
	{ time $$COMMAND ;} 2>./time_outputs/T1_threads_grep.txt ; \
	python $(PY_AVERAGE) ./time_outputs/T1_threads_*.txt > ./results/T1_threads_grep.txt ; \
	rm ./time_outputs/T1_threads_*.txt ; \
	python resuts_to_plot.py ext ./results/*.txt ; \

T2:
	@for alg in $(EXACT_ALG); do \
		echo "começando testes com algoritmo: "$$alg; \
		for i in 0 1 2 3 4; do \
			echo "Teste $$i"; \
			for j in 1 2 3 4 5; do \
				COMMAND="$(PMT) -c -a $$alg $(MEDIUM_PAT) $(ANY_FILE)$$i.txt" ; \
				{ time $$COMMAND ;} 2>./time_outputs/T2_temp_f$$j.txt ; \
			done; \
			echo "done."; \
			python $(PY_AVERAGE) ./time_outputs/T2_temp_f*.txt > ./results/T2_$$alg$$i.txt ; \
			rm ./time_outputs/T2_temp_f*.txt ; \
		done ; \
		echo "Threads em $$alg" ; \
		COMMAND="$(PMT) -c -a $$alg $(MEDIUM_PAT) $(ALL_FILES)" ; \
		{ time $$COMMAND ;} 2>./time_outputs/T2_threads_$$alg.txt ; \
		python $(PY_AVERAGE) ./time_outputs/T2_threads_*.txt > ./results/T2_threads_$$alg.txt ; \
	done
	@for i in 0 1 2 3 4; do \
		echo "Teste grep"; \
		for j in 1 2 3 4 5; do \
			COMMAND="grep -c $(MEDIUM_PAT) $(ANY_FILE)$$i.txt" ; \
			{ time $$COMMAND ;} 2>./time_outputs/T2_temp_f$$j.txt ; \
		done; \
		echo "done."; \
		python $(PY_AVERAGE) ./time_outputs/T2_temp_f*.txt > ./results/T2_grep$$i.txt ; \
		rm ./time_outputs/T2_temp_f*.txt ; \
	done ;
	@echo "Todos os arquivos com grep" ; \
	COMMAND="grep -c $(MEDIUM_PAT) $(ALL_FILES)" ; \
	{ time $$COMMAND ;} 2>./time_outputs/T2_threads_grep.txt ; \
	python $(PY_AVERAGE) ./time_outputs/T2_threads_*.txt > ./results/T2_threads_grep.txt ; \
	rm ./time_outputs/T2_threads_*.txt ; \
	python ./results/resuts_to_plot.py ext ./results/*.txt ; \
	
T3:
	@for alg in $(EXACT_ALG); do \
		echo "começando testes com algoritmo: "$$alg; \
		for i in 0 1 2 3 4; do \
			echo "Teste $$i"; \
			for j in 1 2 3 4 5; do \
				COMMAND="$(PMT) -c -a $$alg $(LONG_PAT) $(ANY_FILE)$$i.txt" ; \
				{ time $$COMMAND ;} 2>./time_outputs/T3_temp_f$$j.txt ; \
			done; \
			echo "done."; \
			python $(PY_AVERAGE) ./time_outputs/T3_temp_f*.txt > ./results/T3_$$alg$$i.txt ; \
			rm ./time_outputs/T3_temp_f*.txt ; \
		done ; \
		echo "Threads em $$alg" ; \
		COMMAND="$(PMT) -c -a $$alg $(LONG_PAT) $(ALL_FILES)" ; \
		{ time $$COMMAND ;} 2>./time_outputs/T3_threads_$$alg.txt ; \
		python $(PY_AVERAGE) ./time_outputs/T3_threads_*.txt > ./results/T3_threads_$$alg.txt ; \
	done
	for i in 0 1 2 3 4; do \
		echo "Teste grep"; \
		for j in 1 2 3 4 5; do \
			COMMAND="grep -c $(LONG_PAT) $(ANY_FILE)$$i.txt" ; \
			{ time $$COMMAND ;} 2>./time_outputs/T3_temp_f$$j.txt ; \
		done; \
		echo "done."; \
		python $(PY_AVERAGE) ./time_outputs/T3_temp_f*.txt > ./results/T3_grep$$i.txt ; \
		rm ./time_outputs/T3_temp_f*.txt ; \
	done ;
	echo "Todos os arquivos com grep" ; \
	COMMAND="grep -c $(LONG_PAT) $(ALL_FILES)" ; \
	{ time $$COMMAND ;} 2>./time_outputs/T3_threads_grep.txt ; \
	python $(PY_AVERAGE) ./time_outputs/T3_threads_*.txt > ./results/T3_threads_grep.txt ; \
	rm ./time_outputs/T3_threads_*.txt ; \
	python ./results/resuts_to_plot.py ext ./results/*.txt ; \

T4:
	@for alg in $(APROX_ALG); do \
		echo "começando testes com algoritmo: $$alg"; \
		for i in 0 1 2; do \
			echo "Teste de dist $$i"; \
			for j in 1 2 3 4 5; do \
				COMMAND="$(PMT) -c -e $$i -a $$alg $(SHORT_PAT) $(DEFAULT_FILE)" ; \
				{ time $$COMMAND ;} 2>./time_outputs/T4_temp_f$$j.txt ; \
			done; \
			echo "done."; \
			python $(PY_AVERAGE) ./time_outputs/T4_temp_f*.txt > ./results/T4_$$alg$$i.txt ; \
			rm ./time_outputs/T4_temp_f*.txt ; \
		done ; \
	done
	for i in 0 1 2 ; do \
		echo "Teste agrep"; \
		for j in 1 2 3 4 5; do \
			COMMAND="agrep -c -$$i $(SHORT_PAT) $(DEFAULT_FILE)" ; \
			{ time $$COMMAND ;} 2>./time_outputs/T4_temp_f$$j.txt ; \
		done; \
		echo "done."; \
		python $(PY_AVERAGE) ./time_outputs/T4_temp_f*.txt > ./results/T4_agrep$$i.txt ; \
		rm ./time_outputs/T4_temp_f*.txt ; \
	done ;
	python ./results/resuts_to_plot.py aprox 2 ./results/*.txt ; \

T5:
	@for alg in $(APROX_ALG); do \
		echo "começando testes com algoritmo: $$alg"; \
		for i in 0 1 2 3 4 5 6; do \
			echo "Teste de dist $$i"; \
			for j in 1 2; do \
				COMMAND="$(PMT) -c -e $$i -a $$alg $(MEDIUM_PAT) $(DEFAULT_FILE)" ; \
				{ time $$COMMAND ;} 2>./time_outputs/T5_temp_f$$j.txt ; \
			done; \
			echo "done."; \
			python $(PY_AVERAGE) ./time_outputs/T5_temp_f*.txt > ./results/T5_$$alg$$i.txt ; \
			rm ./time_outputs/T5_temp_f*.txt ; \
		done ; \
	done
	for i in 0 1 2 3 4 5 6; do \
		echo "Teste agrep"; \
		for j in 1 2; do \
			COMMAND="agrep -c -$$i $(MEDIUM_PAT) $(DEFAULT_FILE)" ; \
			{ time $$COMMAND ;} 2>./time_outputs/T5_temp_f$$j.txt ; \
		done; \
		echo "done."; \
		python $(PY_AVERAGE) ./time_outputs/T5_temp_f*.txt > ./results/T5_agrep$$i.txt ; \
		rm ./time_outputs/T5_temp_f*.txt ; \
	done ; \
	python ./results/resuts_to_plot.py aprox 6 ./results/*.txt ; \

T6:
	@for alg in $(APROX_ALG); do \
		echo "começando testes com algoritmo: $$alg"; \
		for i in 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15; do \
			echo "Teste de dist $$i"; \
			for j in 1; do \
				COMMAND="$(PMT) -c -e $$i -a $$alg $(LONG_PAT) $(DEFAULT_FILE)" ; \
				{ time $$COMMAND ;} 2>./time_outputs/T6_temp_f$$j.txt ; \
			done; \
			echo "done."; \
			python $(PY_AVERAGE) ./time_outputs/T6_temp_f*.txt > ./results/T6_$$alg$$i.txt ; \
			rm ./time_outputs/T6_temp_f*.txt ; \
		done ; \
	done
	for i in 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15; do \
		echo "Teste agrep"; \
		for j in 1; do \
			COMMAND="agrep -c -$$i $(LONG_PAT) $(DEFAULT_FILE)" ; \
			{ time $$COMMAND ;} 2>./time_outputs/T6_temp_f$$j.txt ; \
		done; \
		echo "done."; \
		python $(PY_AVERAGE) ./time_outputs/T6_temp_f*.txt > ./results/T6_agrep$$i.txt ; \
		rm ./time_outputs/T6_temp_f*.txt ; \
	done ; \
	python ./results/resuts_to_plot.py aprox 15 ./results/*.txt ; \

T7:
	@for alg in $(MULT_ALG); do \
		echo "começando testes com algoritmo: "$$alg; \
		for i in 0 1 2 3 4; do \
			echo "Teste $$i"; \
			for j in 1 2 3 4 5; do \
				COMMAND="$(PMT) -c -a $$alg -p $(PATTERN_FILE) $(ANY_FILE)$$i.txt" ; \
				{ time $$COMMAND ;} 2>./time_outputs/T7_temp_f$$j.txt ; \
			done; \
			echo "done."; \
			python $(PY_AVERAGE) ./time_outputs/T7_temp_f*.txt > ./results/T7_$$alg$$i.txt ; \
			rm ./time_outputs/T7_temp_f*.txt ; \
		done ; \
		echo "Threads em $$alg" ; \
		COMMAND="$(PMT) -c -a $$alg -p $(PATTERN_FILE) $(ALL_FILES)" ; \
		{ time $$COMMAND ;} 2>./time_outputs/T7_threads_$$alg.txt ; \
		python $(PY_AVERAGE) ./time_outputs/T7_threads_*.txt > ./results/T7_threads_$$alg.txt ; \
	done
	for i in 0 1 2 3 4; do \
		echo "Teste grep"; \
		for j in 1 2; do \
			COMMAND="grep -c -f $(PATTERN_FILE) $(ANY_FILE)$$i.txt" ; \
			{ time $$COMMAND ;} 2>./time_outputs/T7_temp_f$$j.txt ; \
		done; \
		echo "done."; \
		python $(PY_AVERAGE) ./time_outputs/T7_temp_f*.txt > ./results/T7_grep$$i.txt ; \
		rm ./time_outputs/T7_temp_f*.txt ; \
	done ;
	echo "Todos os arquivos com grep" ; \
	COMMAND="grep -c 119083473f $(PATTERN_FILE) $(ALL_FILES)" ; \
	{ time $$COMMAND ;} 2>./time_outputs/T7_threads_grep.txt ; \
	python $(PY_AVERAGE) ./time_outputs/T7_threads_*.txt > ./results/T7_threads_grep.txt ; \
	rm ./time_outputs/T7_threads_*.txt ; \
	python ./results/resuts_to_plot.py ext ./results/*.txt ; \

T8:
	@for alg in $(MULT_ALG); do \
		echo "começando testes com algoritmo: "$$alg; \
		for i in 1 2 3 4 5 6; do \
			echo "Teste $$i"; \
			for j in 0 1 2; do \
				COMMAND="$(PMT) -c -a $$alg -p $(PI_FILE)$$i.txt $(DEFAULT_FILE)" ; \
				{ time $$COMMAND ;} 2>./time_outputs/T8_temp_f$$j.txt ; \
			done; \
			echo "done."; \
			python $(PY_AVERAGE) ./time_outputs/T8_temp_f*.txt > ./results/T8_$$alg$$i.txt ; \
			rm ./time_outputs/T8_temp_f*.txt ; \
		done ; \
	done
	for i in 1 2 3 4 5 6; do \
		echo "Teste grep"; \
		for j in 0 1 2; do \
			COMMAND="grep -c -f $(PI_FILE)$$i.txt  $(DEFAULT_FILE)" ; \
			{ time $$COMMAND ;} 2>./time_outputs/T8_temp_f$$j.txt ; \
		done; \
		echo "done."; \
		python $(PY_AVERAGE) ./time_outputs/T8_temp_f*.txt > ./results/T8_grep$$i.txt ; \
		rm ./time_outputs/T8_temp_f*.txt ; \
	done ;
	echo "Todos os arquivos com grep" ; \
	python ./results/resuts_to_plot.py mult ./results/*.txt ; \

clean_texts:
	rm ./texts/text_*.txt
clean_results:
	rm ./results/*.txt