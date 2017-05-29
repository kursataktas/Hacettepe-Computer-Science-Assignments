import os, re, sys, math, time, operator
from collections import defaultdict

start = time.clock()

def prob_unigram(i, unigrams_dict, unigrams_len):
	if unigrams_dict[i]:
		return unigrams_dict[i] / unigrams_len
	else:
		return 1 / unigrams_len

def find_unigram_prob(sentence, unigrams_dict, unigrams_len):
	tokens = re.findall(r"\w+(?:[-']\w+)*|'|[-.(]+|\S\w*", sentence)

	total_prob = 0
	for token in tokens:
		total_prob += math.log(prob_unigram(token, unigrams_dict, unigrams_len), 2)
	return 2**total_prob

def get_words(text):
	return re.compile('\w+').findall(text)

def clear_data(text):
	pattern = "[\t\n\r\f\v ]+|</?.*>|\[?.*\]|/?.*/|[0-9]"
	return re.sub(pattern, " ", text).lower()

def calculate_percentage(part, total):
	return (part / total) * 100


# Turkish
###############################################################################################

# Read corpus
raw_text = "";
path = 'data/turkish/'
files = [f for f in os.listdir(path)]
for f_name in files:
	f = open(path + f_name)
	raw_text += f.read()

# remove unnecessary tokens
raw_text_turkish = clear_data(raw_text)

# create n-grams
unigrams_turkish = get_words(raw_text_turkish)

# unique n-gram lists
uniq_unigrams_turkish = list(set(unigrams_turkish))

# length of n-grams ( for speed up the probability calcutations )
unigrams_len_turkish = len(unigrams_turkish)

# unigram frequencies ( word : occurrence count )
unigrams_dict_turkish = defaultdict(int)
for u in unigrams_turkish:
	unigrams_dict_turkish[u] += 1

# English
###############################################################################################

# Read corpus
raw_text = "";
path = 'data/english/'
files = [f for f in os.listdir(path)]
for f_name in files:
	f = open(path + f_name)
	raw_text += f.read()

# remove unnecessary tokens
raw_text_english = clear_data(raw_text)

# create n-grams
unigrams_english = get_words(raw_text_english)

# unique n-gram lists
uniq_unigrams_english = list(set(unigrams_english))

# length of n-grams ( for speed up the probability calcutations )
unigrams_len_english = len(unigrams_english)

# unigram frequencies ( word : occurrence count )
unigrams_dict_english = defaultdict(int)
for u in unigrams_english:
	unigrams_dict_english[u] += 1


# German
########################################################################################################

# Read corpus
raw_text = "";
path = 'data/german/'
files = [f for f in os.listdir(path)]
for f_name in files:
	f = open(path + f_name)
	raw_text += f.read()

# remove unnecessary tokens
raw_text_german = clear_data(raw_text)

# create n-grams
unigrams_german = get_words(raw_text_german)

# unique n-gram lists
uniq_unigrams_german = list(set(unigrams_german))

# length of n-grams ( for speed up the probability calcutations )
unigrams_len_german = len(unigrams_german)

# unigram frequencies ( word : occurrence count )
unigrams_dict_german = defaultdict(int)
for u in unigrams_german:
	unigrams_dict_german[u] += 1


# Read command line input and show the results
########################################################################################################

inp = " ".join(get_words(sys.argv[1])).lower()

result = defaultdict(int)
result["Turkish"] = find_unigram_prob(inp, unigrams_dict_turkish, unigrams_len_turkish)
result["English"] = find_unigram_prob(inp, unigrams_dict_english, unigrams_len_english)
result["German"] = find_unigram_prob(inp, unigrams_dict_german, unigrams_len_german)

total = result["Turkish"] + result["English"] + result["German"]
i = 0
for key,value in sorted(result.items(), key=operator.itemgetter(1), reverse=True):
	if i == 0:
		print("Given string is in\033[92m", key, "\033[0mlanguage.\n\nProbability Details\n----------------------------------------------------")
		i = i + 1
	print(key, ":", "{0:.2f}".format(calculate_percentage(value, total)), "%")

# Print time
stop = time.clock()
print ("\nTotal Time:", format(stop - start, ".2f"), "seconds")