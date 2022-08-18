local __test__ = false
__debug_exe__ = true

local util = require "base.util"
require "match.match_tree.tree_node"

match_tree = {}

function match_tree.new()
        local tree = {}
        tree.tree_node = tree_node.new()

        tree.get_node = function()
                return tree.tree_node
        end

        tree.build_match_tree = function(num)
                local partitions = {}
                function partition(n, max_num, vec)
                        if n == 0 then
                                table.sort(vec)
                                table.insert(partitions, vec)
                        return
                        end
                        if max_num == 0 then
                                table.insert(partitions, vec)
                                return
                        end
                        for i = max_num, 1, -1 do
                                local vec1 = util:deep_copy(vec)
                                table.insert(vec1, i)
                                partition(n - i, math.min(i, n-i), vec1)
                        end
                end

                function reverse(vec, from, to)
                        while from < to do
                                vec[from + 1], vec[to + 1] = vec[to + 1], vec[from + 1]

                                from = from + 1
                                to = to - 1
                        end
                end

                --字典序排列
                --lua下标从1开始 这个要注意
                function dic_sort(vec)
                        local i = #vec - 2
                        local j = #vec - 1
                        local m = 0

                        while i >= 0 do
                                if (vec[i + 1 + 1] > vec[i + 1]) then
                                        break
                                end
                                i = i - 1
                        end
                        if i < 0 then
                                return false
                        end
                        m = i
                        i = i + 1

                        while j > i do
                                if vec[j + 1] > vec[m + 1] then
                                        break
                                end
                                j = j - 1
                        end

                        vec[j + 1], vec[m + 1] = vec[m + 1], vec[j + 1]
                        reverse(vec, m + 1, #vec - 1)
                        return true;
                end

                partition(num, num, {})

                for i,v in ipairs(partitions) do
                        local node = tree.tree_node
                        for k,val in ipairs(v) do
                                local child_node = node.get_child_node(val)
                                if child_node then
                                        node = child_node
                                else
                                        node = node.add_child_node(val)
                                end
                        end
                        while dic_sort(v) do
                                local node1 = tree.tree_node
                                for k,val in ipairs(v) do
                                        local child_node = node1.get_child_node(val)
                                        if child_node then
                                                node1 = child_node
                                        else
                                                node1 = node1.add_child_node(val)
                                        end
                                end
                        end

                end

        end

        return tree
end

if __test__ then
        local tree = match_tree.new()
        tree.build_match_tree(15)
        print(tree.tree_node.dump_nodes(""))


        print("++++++++++++++++++++++")
        print(tree.tree_node.get_child_node(3).get_child_node(1).dump_nodes(""))
end

