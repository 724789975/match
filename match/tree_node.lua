tree_node = {}

function tree_node.new()
        local node = {}
        node.data = 0
        node.father_node = nil
        node.nodes = {}

        --获取子节点
        node.get_child_node = function(cref_key)
                for i,v in ipairs(node.nodes) do
                        if v.data == cref_key then
                                return v
                        end
                end
                return nil
        end

        --获取数据
        function tree_node.get_data()
                return tree_node.data
        end

        --添加子节点
        node.add_child_node = function(cref_data)
                if node.get_child_node(cref_data) == nil then
                        local node_child = tree_node.new()
                        node_child.data = cref_data
                        node_child.father_node = node
                        table.insert(node.nodes, node_child)
                        return node_child
                end
                return nil
        end

        --移除子节点
        node.remove_child_node = function(cref_data)
                for i,v in ipairs(node.nodes) do
                        if v.data == cref_key then
                                table.remove(node.nodes, i)
                                return true
                        end
                end
                return false
        end

        --是否叶子节点
        node.is_leaf_node = function()
                return #node.nodes == 0
        end

        --是否根节点
        node.is_root_node = function()
                return node.father_node == nil
        end

        node.dump_nodes = function()
                local str = ""

                if node.is_leaf_node() then
                        str = str .. node.data .. "\t"
                        local node_f = node.father_node
                        while node_f do
                                if node_f.father_node then
                                        str = str .. node_f.data .. "\t"
                                else
                                        str = str .. "\n----------------------------\n"
                                end
                                node_f = node_f.father_node
                        end
                        if __debug_exe__ then
                                match_log(DEBUG, str)
                        else
                                print(str)
                        end
                else
                        for i,v in pairs(node.nodes) do
                                v.dump_nodes()
                        end
                end

        end

        return node
end

