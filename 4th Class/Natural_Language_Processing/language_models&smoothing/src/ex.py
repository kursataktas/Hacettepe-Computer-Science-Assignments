import os
import re
import math
import time
import random
from collections import defaultdict

start = time.clock()

def prob_unigram(i, unigrams):
	if unigrams_dict[i]:
		return unigrams_dict[i] / unigrams_len
	else:
		return 1 / len(uniq_unigrams)

def prob_bigram(i, j, bigrams, unigrams, smoothed = True):
	if bigrams_dict[(i, j)]:
		return bigrams_dict[(i, j)] / unigrams_dict[i]
	elif smoothed:
		return 1 / (unigrams_dict[i] + len(uniq_unigrams))
	else:
		return 0

def prob_trigram(i, j, k, trigrams, bigrams, smoothed = False):
	if trigrams_dict[(i, j, k)]:
		return trigrams_dict[(i, j, k)] / bigrams_dict[(i, j)]
	elif smoothed:
		return 1 / (bigrams_dict[(i, j)] + len(uniq_bigrams))
	else:
		return 0

def find_unigram_prob(sentence):
	tokens = re.findall(r"\w+(?:[-']\w+)*|'|[-.(]+|\S\w*", sentence)

	total_prob = 0
	for token in tokens:
		total_prob += math.log(prob_unigram(token, unigrams), 2)
	return 2**total_prob

def find_bigram_prob(sentence):
	tokens = re.findall(r"\w+(?:[-']\w+)*|'|[-.(]+|\S\w*", sentence)

	total_prob = 0
	k = 0;
	for token in tokens:
		if(tokens[0] == token):
			for i in ["!", ".", "...", "?"]:
				total_prob += prob_bigram(i, token, bigrams, unigrams)
			total_prob = math.log(total_prob, 2)
		else:
			total_prob += math.log(prob_bigram(tokens[k - 1], token, bigrams, unigrams), 2)
		k = k + 1

	return 2**total_prob

def find_trigram_prob(sentence):
	tokens = re.findall(r"\w+(?:[-']\w+)*|'|[-.(]+|\S\w*", sentence)

	total_prob = 0
	k = 0;
	for token in tokens:
		if(tokens[0] == token):
			for i in ["!", ".", "...", "?"]:
				total_prob += prob_bigram(i, token, bigrams, unigrams)
			total_prob = math.log(total_prob, 2)
		elif k < 2:
			total_prob += math.log(prob_bigram(tokens[k - 1], token, bigrams, unigrams), 2)
		else:
			total_prob += math.log(prob_trigram(tokens[k - 2], tokens[k - 1], token, trigrams, bigrams), 2)
		k = k + 1

	return 2**total_prob

def create_sentence_unigram():
	s = ""
	for i in range(0, 20):
		s += random.choice(uniq_unigrams) + " "
		if(s[-2] in ["!", ".", "...", "?"]):
			break
	return s

def create_sentence_bigram():
	s = ""
	s += random.choice(starting_words) + " "
	for i in range(0, 19):
		s += random.choice([y for (x, y) in uniq_bigrams if x == s.rsplit(None, 1)[-1]]) + " "
		if(s[-2] in ["!", ".", "...", "?"]):
			break
	return s

def create_sentence_trigram():
	s = ""
	s += random.choice(starting_words) + " "
	s += random.choice([y for (x, y) in uniq_bigrams if x == s.rsplit(None, 1)[-1]]) + " "

	if(s[-2] in ["!", ".", "...", "?"]):
			return s

	for i in range(0, 18):
		s += random.choice([z for (x, y, z) in uniq_trigrams if (x == s.rsplit(None, 2)[-2] and y == s.rsplit(None, 2)[-1])]) + " "
		if(s[-2] in ["!", ".", "...", "?"]):
			break
	return s

def unigram_perplexity(tokens):
	perplexity = 0;
	for token in tokens:
		perplexity += math.log(prob_unigram(token, unigrams), 2)
	return 2**( (-1 / len(tokens)) * perplexity)

def bigram_perplexity(tokens):
	perplexity = 0;
	k = 0
	for token in tokens:
		if(tokens[0] == token):
			for i in ["!", ".", "...", "?"]:
				perplexity += prob_bigram(i, token, bigrams, unigrams)
			perplexity = math.log(perplexity, 2)
		else:
			perplexity += math.log(prob_bigram(tokens[k - 1], token, bigrams, unigrams), 2)
		k = k + 1
	return 2**( (-1 / len(tokens)) * perplexity)

def trigram_perplexity(sentence):
	perplexity = 0;
	k = 0;
	for token in tokens:
		if tokens[0] == token:
			for i in ["!", ".", "...", "?"]:
				perplexity += prob_bigram(i, token, bigrams, unigrams)
			perplexity = math.log(perplexity, 2)
		elif k < 2:
			perplexity += math.log(prob_bigram(tokens[k - 1], token, bigrams, unigrams), 2)
		else:
			perplexity += math.log(prob_trigram(tokens[k - 2], tokens[k - 1], token, trigrams, bigrams, True), 2)
		k = k + 1

	return 2**( (-1 / len(tokens)) * perplexity)

# Read corpus
raw_text = "";
path = 'data/comedies/'
files = [f for f in os.listdir(path)]
for f_name in files:
	f = open(path + f_name)
	raw_text += f.read()

# remove unnecessary whitespaces
pattern = "[\t\n\r\f\v ]+|</?.*>"
raw_text = re.sub(pattern, " ", raw_text)

# create n-grams
unigrams = re.findall(r"\w+(?:[-']\w+)*|'|[-.(]+|\S\w*", raw_text)
bigrams = list(zip(unigrams, unigrams[1:]))
trigrams = list(zip(unigrams, unigrams[1:], unigrams[2:]))

# unique n-gram lists
uniq_unigrams = list(set(unigrams))
uniq_bigrams  = list(set(bigrams))
uniq_trigrams = list(set(trigrams))

# length of n-grams ( for speed up the probability calcutations )
unigrams_len = len(unigrams)
bigrams_len = len(bigrams)
trigrams_len = len(trigrams)

# unigram frequencies ( word : occurrence count )
unigrams_dict = defaultdict(int)
for u in unigrams:
	unigrams_dict[u] += 1

# bigram frequencies
bigrams_dict = defaultdict(int)
for u in bigrams:
	bigrams_dict[u] += 1

# trigram frequencies
trigrams_dict = defaultdict(int)
for u in trigrams:
	trigrams_dict[u] += 1

# word list which used in start of a sentence  
starting_words = [y for (x, y) in uniq_bigrams if x in ["!", ".", "...", "?"]]


# TASK 1
print("Task - 1\n")
s1 = "To work or not to work, that is the problem!"
print(s1, "\n", "-> P() : ", find_bigram_prob(s1), "\n\n")

s1 = "Shall sleep more, Theodore shall sleep more."
print(s1, "\n", "-> P() : ", find_bigram_prob(s1), "\n\n")

s1 = "It does not matter how slowly you go so long as you do not stop."
print(s1, "\n", "-> P() : ", find_bigram_prob(s1), "\n\n")

s1 = "Imagination is more important than knowledge..."
print(s1, "\n", "-> P() : ", find_bigram_prob(s1), "\n\n")

s1 = "Thou seest, the heavens, as troubled with man's act"
print(s1, "\n", "-> P() : ", find_bigram_prob(s1), "\n\n")

# TASK 2
print("Task - 2\n")
print("Sentences With Unigrams\n--------------------")
for i in range(0, 10):
	s = create_sentence_unigram()
	s_p = find_unigram_prob(s)
	print(i + 1, ")", s)
	print("P:", s_p, "\n")

print("Sentences With Bigrams\n--------------------")
for i in range(0, 10):
	s = create_sentence_bigram()
	s_p = find_bigram_prob(s)
	print(i + 1, ")", s)
	print("P:", s_p, "\n")

print("Sentences With Trigram\n--------------------")
for i in range(0, 10):
	s = create_sentence_trigram()
	s_p = find_trigram_prob(s)
	print(i + 1, ")", s)
	print("P:", s_p, "\n")


# Task - 3
raw_text = "";
path = 'data/historical/'
files = [f for f in os.listdir(path)]
for f_name in files:
	f = open(path + f_name)
	raw_text += f.read()

pattern = "[\s ]+|</?.*>"
raw_text = re.sub(pattern, " ", raw_text)

tokens = re.findall(r"\w+(?:[-']\w+)*|'|[-.(]+|\S\w*", raw_text)

print("Task - 3\n")
print("Perplexities\n")
print("Unigram:", unigram_perplexity(tokens))
print("Bigram:", bigram_perplexity(tokens))
print("Trigram:", trigram_perplexity(tokens))


# Print time
stop = time.clock()
print ("\nTotal Time:", format(stop - start, ".2f"), "seconds")