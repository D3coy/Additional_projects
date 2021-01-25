import random
#    8  3  6  5  -  5  7  9  4  -  2  5  6  6  -  0  8  1  7  [password test]
#    0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18  [numbers in arr]

class keygen_k:
    def gen_part(self):
        genr = lambda: str(random.randint(0, 9))
        key_part = ''.join([genr() for x in range(4)])

        return key_part

    def shr(self, key):
        # Divide key on blocks
        g1 = key[0:4]
        g2 = key[5:9]
        g3 = key[10:14]
        g4 = key[15:19]

        # bias alg from crackme
        shrkx = self.sum_alg(g1) + self.sum_alg(g2) + self.sum_alg(g3) + self.sum_alg(g4)
        shrkx = shrkx >> 2
        return shrkx

    
    def sum_alg(self, key_part):
        num_1 = key_part[0]
        num_2 = key_part[1]
        num_3 = key_part[2]
        num_4 = key_part[3]

        # sum algorythm from crackme (K_crackme_3.png), counting in base10, not 0x base16
        summ = ord(num_1) + ord(num_2) + ord(num_3) + ord(num_4) + ord(num_4) + ord(num_4)
        summ_final = ord(num_4) + summ - int('0x150', 16)

        return summ_final


    def check_key(self, key):
        counter = 0

        shr_key = self.shr(key)
        if self.sum_alg(key[0:4]) != shr_key or self.sum_alg(key[5:9]) != shr_key or self.sum_alg(key[10:14]) != shr_key or self.sum_alg(key[15:19]) != shr_key:
            return False

        while counter < 4:
            # check 1st in every grp, 2nd, 3rd, 4th number in generated password
            first_grp = 0 + counter
            second_grp = 5 + counter
            third_grp = 10 + counter
            fourth_grp = 15 + counter
            
            # Check key 1 symb in group with other 1 in group
            if int(key[first_grp]) == int(key[second_grp]):
                return False
            elif int(key[second_grp]) == int(key[third_grp]):
                return False
            elif int(key[third_grp]) == int(key[fourth_grp]):
                return False

            counter += 1
        else:
            return True     # if all clear ..


    def gen_key(self):
        result_key = [0 for x in range(3)]
        
        for i in range(4):
            if i < 3:
                result_key[i] = (self.gen_part() + '-')
            else:
                result_key.append(self.gen_part())
        
        result_key = ''.join(result_key)
        
        return result_key


    def main(self):
        while(True):
            key = self.gen_key()
            print("[INFO] Checking key: {}".format(key))

            if self.check_key(key) == False:
                print("[ERROR] Wrong key: {}".format(key))
            else:
                print("[INFO] Key is correct: {}".format(key))
                break
        

kg_o = keygen_k()
kg_o.main()