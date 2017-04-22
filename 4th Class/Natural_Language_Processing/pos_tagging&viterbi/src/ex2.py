import os
import re
from collections import defaultdict

# Task 2.1
def initial_tag_prob(tag, bigram_tags_freq, total_tag_count):
	"('.', '...', '!', '?') --> these puncations used as a sentence boundary. There are 2 tags for them '.' and '.-hl' "
	total_count =  	bigram_tags_freq[('.', tag)] + bigram_tags_freq[('.-hl', tag)]
	return total_count / total_tag_count

# Task 2.2
def transition_prob(t1, t2, bigram_tags_freq, total_tag_count):
	"Calculates transition probabilities p(t2|t1)"
	return bigram_tags_freq[(t1, t2)] / total_tag_count

# Task 2.3
def emission_prob(t, w, words_tags, tags):
	"Calculates emission probabilites of p(w|t)"
	return words_tags[w][t] / tags[t]

def best_tag(w, words_tags, tags):
	"Fetches best tag for given word (w)"
	max_prob = -1
	for t in words_tags[w]:
		temp = emission_prob(t, w, words_tags, tags)
		if temp > max_prob:
			best_tag = t 
			max_prob = temp
	return best_tag

# Task 4
def viterbi(sen, words_tags, bigram_tags_freq, tags, total_tag_count):
	"Implementation of viterbi algorithm for finding best POS-tagging sequence"
	v = []
	bp = []
	sen = list(sen.split())

	# initial
	temp_v = {}
	temp_bp = {}
	for t in words_tags[sen[0]]:
		temp_v[t] = initial_tag_prob(t, bigram_tags_freq, total_tag_count)
		temp_bp[t] = "<s>"
	v.append(temp_v)
	bp.append(temp_bp)

	for w in sen[1:]:
		temp_v = {}
		temp_bp = {}
		prev_v = v[-1]

		for t in words_tags[w]:
			best_prev = 0
			best_prob = 0
			for prev_tag in prev_v.keys():
				temp_prob = transition_prob(prev_tag, tag, bigram_tags_freq, total_tag_count) * emission_prob(t, w, words_tags, tags)
				if(temp_prob > best_prob):
					best_prev = prev_tag
					best_prob = temp_prob
			temp_v[t] = best_prob
			temp_bp[t] = best_prev

		v.append(temp_v)
		bp.append(temp_bp)
	
	last_tag = max(v[-1], key=v[-1].get)
	tag_seq = [last_tag]
	while bp:
		last_tag = bp[-1][last_tag]
		tag_seq.append(last_tag)
		bp = bp[:-1]

	return list(reversed(tag_seq[:-1]))

# Read corpus
raw_text = "";
path = 'brown/'
files = [f for f in os.listdir(path)]
for f_name in files:
	f = open(path + f_name)
	raw_text += f.read()

pattern = "[\t\n\r\f\v ]+"
raw_text = re.sub(pattern, " ", raw_text).lower()

# Task 1
words_tags = defaultdict(lambda: defaultdict(int))
tags = defaultdict(int)
tag_sequence = []
for word_tag in raw_text.split():
	paired = word_tag.rfind("/")
	
	tag = word_tag[paired+1:]
	tags[tag] += 1
	tag_sequence.append(tag)
	words = word_tag[:paired]

	if words.count('/') == 1:
		# Maybe word is '/' character ? if so check the length
		if len(words) != 1:
			w = words.rfind('/')
			w1 = words[:w]
			w2 = words[w+1:]
			words_tags[w1][tag] += 1
			words_tags[w2][tag] += 1
		else:
			words_tags[words][tag] += 1
	else:
		words_tags[words][tag] += 1

bigram_tags_freq = defaultdict(int)
for u in list(zip(tag_sequence, tag_sequence[1:])):
	bigram_tags_freq[u] += 1


# Task 3
with open("input_tokens.txt") as f:
    rows = f.readlines()

s = ""
for line in rows:
	line = re.sub(pattern, " ", line).lower()
	for word in line.split():
		s = s + word + "/" + best_tag(word, words_tags, tags) + " "
	s += "\n"
with open("output_tokens.txt", 'w') as f:
		f.write(s)
		f.close()

# Task 4
with open("test_set.txt") as f:
    rows = f.readlines()

s = ""
for line in rows:
	viterbi_tags = viterbi(line.lower(), words_tags, bigram_tags_freq, tags, len(tag_sequence))
	words = line.split()
	
	i = 0
	for x in viterbi_tags:
		s = s + words[i] + "/" + x + " "
		i += 1
	s += "\n"

with open("output_set.txt", 'w') as f:
		f.write(s)
		f.close()

